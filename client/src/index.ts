import { RabbitMQClient } from "./rabbitClient";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();
await rabbitInstance.createStream(client, "streamtest");
const publisher = await rabbitInstance.createPublisher(client, "streamtest");


const message = {
  clientID: "client-1",
  currentReading: 42,
  unix: Math.floor(Date.now() / 1000),
  fwVersion: "1.0.0",
  unit: "kWh"
}

console.log("Publishing messages...");
await publisher.send(Buffer.from(JSON.stringify(message)));