// const Alexa = require('ask-sdk-core');
// const AWS = require('aws-sdk');

import * as Alexa from 'ask-sdk-core';
import AWS from 'aws-sdk';


const IotData = new AWS.IotData({endpoint: 'a2dmh18tmfqz8b-ats.iot.us-east-2.amazonaws.com'});
const dynamoDB = new AWS.DynamoDB.DocumentClient();

AWS.config.update({ region: 'us-west-2' });

let currUser;
let thingDevice = 'dispensador';


function createStartDispense(waterTarget) {
    return {
        thingName: thingDevice,
        payload: JSON.stringify({
            state: {
                desired: {
                    pumpState: "dispense",
                    waterTarget: waterTarget
                }
            }
        })
    };
}


async function checkSession() {
    try {
        const DB_dispenser = 'Session';
        const params = {
            TableName: DB_dispenser,
            KeyConditionExpression: 'SessionId = :session',
            ExpressionAttributeValues: {
                ':session': 0
            }
        };
        
        const result = await dynamoDB.query(params).promise();
        return result.Items || [];
    } catch (error) {
        console.error('Error al realizar la consulta en DynamoDB:', error);
        throw new Error('Error al consultar la base de datos.');
    }
}

async function getUser(userId) {
    try {
        const DB_dispenser = 'Users';
        const params = {
            TableName: DB_dispenser,
            KeyConditionExpression: 'UserId = :user',
            ExpressionAttributeValues: {
                ':user': userId
            }
        };
        
        const result = await dynamoDB.query(params).promise();
        return result.Items || [];
    } catch (error) {
        console.error('Error al realizar la consulta en DynamoDB:', error);
        throw new Error('Error al consultar la base de datos.');
    }
}

function getShadowPromise() {
    const params = {
        thingName : thingDevice,
    };
    
    return new Promise((resolve, reject) => {
        IotData.getThingShadow(params, (err, data) => {
            if (err) {
                console.log(err, err.stack);
                reject('Failed to get thing shadow ${err.errorMessage}');
            } else {
                resolve(JSON.parse(data.payload));
            }
        });
    });
}


const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    async handle(handlerInput) {
        let speakOutput = 'error';
        
        try {
            const response = await checkSession();
            console.log(response);
            
            if(response.length === 0){
                speakOutput = "¡Hola y bienvenido! Soy tu dispensador de agua. Antes de continuar, por favor inicie sesion con un usuario";
            }else{
                currUser = await getUser(response[0].UserId);
                speakOutput = `¡Hola y bienvenido ${currUser[0].Name}! Puedes pedirme que sirva agua cuando lo necesites. `;
            }
            
        } catch (e) {
            speakOutput = 'Hubo un error al leer los credenciales.';
        }

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const DispenseCustomIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'DispenseCustomIntent';
    },
    handle(handlerInput) {
        
        const slots = handlerInput.requestEnvelope.request.intent.slots;
        const mlOfWater = parseInt(slots.mlOfWater.value);
        
        let speakOutput;

        if (mlOfWater && !isNaN(mlOfWater) && mlOfWater > 0 && mlOfWater <= 800) {

            IotData.updateThingShadow(createStartDispense(mlOfWater), function(err, data) {
                if (err) console.log(err);
            });

            speakOutput = `Sirviendo ${mlOfWater} mililitros de agua.`;
        } else {
            speakOutput = "Por favor, pide una cantidad válida de agua entre 1 y 800 mililitros.";
        }

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};


const DispenseFullIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'DispenseFullIntent';
    },
    handle(handlerInput) {

        IotData.updateThingShadow(createStartDispense(200), function(err, data) {
            if (err) console.log(err);
        });

        const speakOutput = `Sirviendo un vaso lleno de agua.`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }   
};

const DispenseHalfIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'DispenseHalfIntent';
    },
    handle(handlerInput) {
        
        IotData.updateThingShadow(createStartDispense(100), function(err, data) {
            if (err) console.log(err);
        });

        const speakOutput = `Sirviendo medio vaso de agua.`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }   
};

const ChangeDeviceIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'ChangeDeviceIntent';
    },
    handle(handlerInput) {
        
        const slots = handlerInput.requestEnvelope.request.intent.slots;
        thingDevice = slots.thing.value;   

        const speakOutput = `Cambiando a ${thingDevice}.`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }   
};

const RemainingIntentHandler = {
     canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'RemainingIntent';
    },
    async handle(handlerInput) {
        var remainingWater = 0;
        await getShadowPromise().then((result) => remainingWater = result.state.reported.remainingWater);
        
        const speakOutput = `Quedan aproximadamente ${remainingWater} mililitros de agua`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }   
};


const HelpIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.HelpIntent';
    },
    handle(handlerInput) {
        const speakOutput = '¡Hola! Soy tu dispensador de agua. Puedo ayudarte a servir agua cuando lo necesites. Simplemente di "quiero un vaso de agua" o "quiero x mililitros de agua" y lo haré por ti. ¿En qué más te puedo ayudar?';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};


const CancelAndStopIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.CancelIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.StopIntent');
    },
    handle(handlerInput) {
        const speakOutput = 'Adios!';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const FallbackIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.FallbackIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'Lo siento, no entiendo la instruccion. Por favor, inténtelo de nuevo.';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const SessionEndedRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'SessionEndedRequest';
    },
    handle(handlerInput) {
        console.log(`~~~~ Session ended: ${JSON.stringify(handlerInput.requestEnvelope)}`);
        return handlerInput.responseBuilder.getResponse(); 
    }
};

const IntentReflectorHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest';
    },
    handle(handlerInput) {
        const intentName = Alexa.getIntentName(handlerInput.requestEnvelope);
        const speakOutput = `Usted acaba de desencadenar ${intentName}`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        const speakOutput = 'Lo siento, he tenido problemas para hacer lo que me pedías. Por favor, inténtelo de nuevo.';
        console.log(`~~~~ Error handled: ${JSON.stringify(error)}`);

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

export const handler = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        DispenseCustomIntentHandler,
        DispenseFullIntentHandler,
        DispenseHalfIntentHandler,
        ChangeDeviceIntentHandler,
        RemainingIntentHandler,
        HelpIntentHandler,
        CancelAndStopIntentHandler,
        FallbackIntentHandler,
        SessionEndedRequestHandler,
        IntentReflectorHandler)
    .addErrorHandlers(
        ErrorHandler)
    .withCustomUserAgent('sample/hello-world/v1.2')
    .lambda();