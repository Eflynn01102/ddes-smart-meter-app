import rabbit from "rabbitmq-stream-js-client";
import type { Message } from "rabbitmq-stream-js-client/dist/publisher";

export class RabbitMQClient {
  static instance: RabbitMQClient;

  private constructor() {}

  public static get Instance(): RabbitMQClient {
    if (!RabbitMQClient.instance) {
      RabbitMQClient.instance = new RabbitMQClient();
    }
    return RabbitMQClient.instance;
  }

  public async connectionClient() {
    console.log("Connecting to RabbitMQ...");
    const client  = await rabbit.connect({
      hostname: "127.0.0.1",
      port: 5552,
      username: "guest",
      password: "guest",
      vhost: "/",
    })
    return client;
  }

  public async createStream(client: rabbit.Client, topicName: string) {
    console.log("Creating stream...");
    await client.createStream({
      stream: topicName,
      arguments: {
        "max-length-bytes": 100000000,
      }
    })
  }

  public async createConsumer(client: rabbit.Client, topicName: string, offset: rabbit.Offset, onMessage: (message: Message) => void) {
    await client.declareConsumer({
      stream: topicName,
      offset: offset
    },
      onMessage
    )
  }
}