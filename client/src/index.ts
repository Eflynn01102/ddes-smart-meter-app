import type { Publisher } from "rabbitmq-stream-js-client";
import { RabbitMQClient } from "./rabbitClient";
import { rabbitMessage } from "./types/message";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();
await rabbitInstance.createStream(client, "streamtest");
await rabbitInstance.createStream(client, "streamtest2");

const publisher = await rabbitInstance.createPublisher(client, "streamtest");
const publisher2 = await rabbitInstance.createPublisher(client, "streamtest2");

setInterval(async () => {
  
})

const message: rabbitMessage = {
  clientID: "client-1",
  currentReading: 43,
  unix: Math.floor(Date.now() / 1000),
  fwVersion: "1.0.0",
  unit: "kWh",
  encryption: "HMAC-SHA256",
}

async function messaghandler(publisher: Publisher, message: rabbitMessage) {
  console.log("Publishing messages...");
  const pub = await publisher.send(Buffer.from(JSON.stringify(message)));
  if (pub.sent){
    console.log("Message sent successfully");
  }
}

await publisher.send(Buffer.from(JSON.stringify(message)));

await publisher2.send(Buffer.from("Hello from stream 2!"));