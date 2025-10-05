import type { Publisher } from "rabbitmq-stream-js-client";
import { RabbitMQClient } from "./components/rabbitClient";
import { generateRandomNumber, generateRandomNumberInRange } from "./components/randomNumberGen";
import { rabbitMessage } from "./types/message";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();
await rabbitInstance.createStream(client, "streamtest");
await rabbitInstance.createStream(client, "streamtest2");

const publisher = await rabbitInstance.createPublisher(client, "streamtest");
const publisher2 = await rabbitInstance.createPublisher(client, "streamtest2");

const publisherArray = [publisher, publisher2];

const message: rabbitMessage = {
  clientID: "client-1",
  currentReading: 43,
  unix: Math.floor(Date.now() / 1000),
  fwVersion: "1.0.0",
  unit: "kWh",
  signature: "HMAC-SHA256",
}

setInterval(async () => {
  const randomNum = generateRandomNumber(2);
  const currentPub = publisherArray.at(randomNum)
  if (!currentPub) 
  {
    console.error("No publisher found");
    return;
  }
  message.clientID = `client-${randomNum + 1}`;
  message.currentReading += generateRandomNumberInRange(0, 100); // need to figure out how to not send a smaller number then the one before
  message.unix = Math.floor(Date.now() / 1000);
  // need to add signature generation
  await messaghandler(currentPub, message);
}, 5000)

async function messaghandler(publisher: Publisher, message: rabbitMessage) {
  console.log("Publishing messages...");
  const pub = await publisher.send(Buffer.from(JSON.stringify(message)));
  console.log("Message sent successfully");
}