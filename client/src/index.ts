import type { Publisher } from "rabbitmq-stream-js-client";
import { RabbitMQClient } from "./components/rabbitClient";
import { generateRandomNumber, generateRandomNumberInRange } from "./components/randomNumberGen";
import { rabbitMessage } from "./types/message";
import { createHmacSignature } from "./components/hmac";

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
  message.currentReading = await readingHandler(randomNum + 1);
  message.unix = Math.floor(Date.now() / 1000);
  message.signature = createHmacSignature(message.clientID, message.currentReading.toString(), message.unix.toString());
  await messaghandler(currentPub, message);
}, 5000)

async function readingHandler(index: number): Promise<number> {
  const lastReading = rabbitInstance.getLastReadingAtIndex(index);
  const currentReading = generateRandomNumberInRange(lastReading, 100);
  await rabbitInstance.insertAtIndex(index, currentReading);
  return currentReading;
}

async function messaghandler(publisher: Publisher, message: rabbitMessage): Promise<void> {
  console.log("Publishing messages...");
  await publisher.send(Buffer.from(JSON.stringify(message)));
  console.log("Message sent successfully");
}