import rabbit from "rabbitmq-stream-js-client";

export class RabbitMQClient {
  static instance: RabbitMQClient;

  private publisherLastMessage: number[] = [0, 0];

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

  public async insertAtIndex(index: number, item: number) {
    [...this.publisherLastMessage.slice(0, index), item, ...this.publisherLastMessage.slice(index)];
  }

  public getLastReadingAtIndex(index: number): number {
    return this.publisherLastMessage.at(index) || 0;
  }

}