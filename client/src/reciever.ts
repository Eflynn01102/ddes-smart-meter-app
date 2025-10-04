import { RabbitMQClient } from "./rabbitClient";
import rabbit from "rabbitmq-stream-js-client";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();

await client.declareConsumer({
  stream: "streamtest",
  offset: rabbit.Offset.last()},
  (message) => {
    console.log("Received message:", message.content.toString());
  }
)

