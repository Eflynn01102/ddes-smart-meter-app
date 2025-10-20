import type { rabbitMessage } from "@client/config/src/message";
import type { Publisher } from "rabbitmq-stream-js-client";
import rabbit from "rabbitmq-stream-js-client";
import { generateRandomNumberInRange } from "./randomNumberGen";

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
		const client = await rabbit.connect({
			hostname: "127.0.0.1",
			port: 5552,
			username: "guest",
			password: "guest",
			vhost: "/",
		});
		return client;
	}

	public async createStream(client: rabbit.Client, topicName: string) {
		console.log("Creating stream...");
		await client.createStream({
			stream: topicName,
			arguments: {
				"max-length-bytes": 100000000,
			},
		});
	}

	public async createPublisher(client: rabbit.Client, topicName: string) {
		console.log("Creating Publisher...");
		const publisher = await client.declarePublisher({
			stream: topicName,
		});
		return publisher;
	}

	private async insertAtIndex(index: number, item: number) {
		this.publisherLastMessage[index] = item;
	}

	private getLastReadingAtIndex(index: number): number {
		return this.publisherLastMessage.at(index) || 0;
	}

	public async readingHandler(index: number): Promise<number> {
		const lastReading = this.getLastReadingAtIndex(index);
		const currentReading = generateRandomNumberInRange(
			lastReading,
			lastReading + 50,
		);
		await this.insertAtIndex(index, currentReading);
		return currentReading;
	}

	public async messagehandler(
		publisher: Publisher,
		message: rabbitMessage,
	): Promise<void> {
		console.log("Publishing messages...");
		await publisher.send(Buffer.from(JSON.stringify(message)));
		console.log("Message sent successfully");
	}
}
