import rabbit from "rabbitmq-stream-js-client";
import { RabbitMQClient } from "../components/rabbitClient";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();

await client.declareConsumer(
	{
		stream: "streamtest",
		offset: rabbit.Offset.last(),
	},
	(message) => {
		console.log("Received message:", message.content.toString());
	},
);

await client.declareConsumer(
	{
		stream: "streamtest2",
		offset: rabbit.Offset.last(),
	},
	(message) => {
		console.log("Received message on stream 2:", message.content.toString());
	},
);
