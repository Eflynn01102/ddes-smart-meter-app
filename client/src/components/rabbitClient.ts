import rabbit from "rabbitmq-stream-js-client";

export class RabbitMQClient {
  static instance: RabbitMQClient;

  public publisherLastMessage: number = 0;

  //need to add 11 more

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

  public async createPublisher(client: rabbit.Client, topicName: string) {
    console.log("Creating Publisher...");
    const publisher = await client.declarePublisher({
      stream: topicName,
    })
    return publisher;
  }

}