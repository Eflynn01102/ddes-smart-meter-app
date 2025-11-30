import type { rabbitMessage } from "@client/config/src/message";
import type { Publisher } from "rabbitmq-stream-js-client";
import {
	port,
	host,
	server,
	sendCurrentUsageToClients,
} from "@/client/NetworkClient";
import { createHmacSignature } from "../src/utils/hmac";
import { RabbitMQClient } from "../src/utils/rabbitClient";
import { generateRandomNumber } from "../src/utils/randomNumberGen";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();

const endPointTopic = "client";

// create 12 streams
for (let i = 0; i <= 11; i++) {
	try {
		await rabbitInstance.createStream(client, `${endPointTopic}${i}`);
	} catch (error) {
		console.error("Error creating stream:", error);
	}
}

const publisherArray: Publisher[] = [];

// create 12 publishers
for (let i = 0; i <= 11; i++) {
	try {
		const tempPublisher = await rabbitInstance.createPublisher(
			client,
			`${endPointTopic}${i}`,
		);
		if (!tempPublisher) break;
		publisherArray.push(tempPublisher);
	} catch (error) {
		console.error("Error creating publisher:", error);
	}
}

const message: rabbitMessage = {
	clientId: "client-0",
	currentReading: 43,
	unix: Math.floor(Date.now() / 1000),
	fwVersion: "1.0.0",
	unit: "kWh",
	signature: "HMAC-SHA256",
};

setInterval(async () => {
	const randomNum = generateRandomNumber(12);
	const currentPub = publisherArray.at(randomNum);
	if (!currentPub) {
		console.error("No publisher found");
		return;
	}
	message.clientId = `client-${randomNum + 1}`;
	message.currentReading = await rabbitInstance.readingHandler(randomNum + 1);
	message.unix = Math.floor(Date.now() / 1000);
	message.signature = createHmacSignature(
		message.clientId,
		message.currentReading.toString(),
		message.unix.toString(),
	);
	await rabbitInstance.messagehandler(currentPub, message);
	sendCurrentUsageToClients({
		clientId: message.clientId,
		currentUsage: message.currentReading,
	});
}, 5000);
server.listen(port, host, () => {
	console.log(`Server running at http://${host}:${port}`);
});
