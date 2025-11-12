import type { rabbitMessage } from "@client/config/src/message";
import type { Publisher } from "rabbitmq-stream-js-client";
import { port, server, sendCurrentUsageToClients } from "@/client/NetworkClient";
import { createHmacSignature } from "@/utils/hmac";
import { RabbitMQClient } from "@/utils/rabbitClient";
import { generateRandomNumber } from "@/utils/randomNumberGen";

const rabbitInstance = RabbitMQClient.Instance;
const client = await rabbitInstance.connectionClient();

const endPointTopic = "client";

// create 12 streams
for (let i = 0; i <= 11; i++) {
	await rabbitInstance.createStream(client, `${endPointTopic}${i}`);
}

const publisherArray: Publisher[] = [];

// create 12 publishers
for (let i = 0; i <= 11; i++) {
	const tempPublisher = await rabbitInstance.createPublisher(
		client,
		`${endPointTopic}${i}`,
	);
	if (!tempPublisher) break;
	publisherArray.push(tempPublisher);
}

const message: rabbitMessage = {
	clientID: "client-0",
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
	message.clientID = `client-${randomNum + 1}`;
	message.currentReading = await rabbitInstance.readingHandler(randomNum + 1);
	message.unix = Math.floor(Date.now() / 1000);
	message.signature = createHmacSignature(
		message.clientID,
		message.currentReading.toString(),
		message.unix.toString(),
	);
	await rabbitInstance.messagehandler(currentPub, message);
	sendCurrentUsageToClients({
		clientId: message.clientID,
		currentUsage: message.currentReading,
	})
}, 5000);

server.listen(port, () => {
	console.log("Server is running on localhost:3000");
});
