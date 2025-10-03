import rabbit from "rabbitmq-stream-js-client";

console.log("Connecting to RabbitMQ...");
const client  = await rabbit.connect({
  hostname: "127.0.0.1",
  port: 5552,
  username: "guest",
  password: "guest",
  vhost: "/",
})

await client.declareConsumer({
  stream: "streamtest",
  offset: rabbit.Offset.last()},
  (message) => {
    console.log("Received message:", message.content.toString());
  }
)