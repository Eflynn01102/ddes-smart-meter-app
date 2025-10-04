import { RabbitMQClient } from "./rabbitClient";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();
await rabbitInstance.createStream(client, "streamtest");
await rabbitInstance.createStream(client, "streamtest2");
const publisher = await rabbitInstance.createPublisher(client, "streamtest");
const publisher2 = await rabbitInstance.createPublisher(client, "streamtest2");

const message = {
  clientID: "client-1",
  currentReading: 43,
  unix: Math.floor(Date.now() / 1000),
  fwVersion: "1.0.0",
  unit: "kWh"
}

console.log("Publishing messages...");
await publisher.send(Buffer.from(JSON.stringify(message)));

await publisher2.send(Buffer.from("Hello from stream 2!"));