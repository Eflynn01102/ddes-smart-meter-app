import rabbit from "rabbitmq-stream-js-client";

console.log("Connecting to RabbitMQ...");
const client  = await rabbit.connect({
  hostname: "localhost",
  port: 5552,
  username: "guest",
  password: "guest",
  vhost: "/",
})

console.log("Creating stream...");
await client.createStream({
  stream: "streamtest",
  arguments: {
    "max-length-bytes": 100000000,
  }
})

console.log("Declaring publisher...");
const publisher = await client.declarePublisher({
  stream: "streamtest",
})

console.log("Publishing messages...");
await publisher.send(Buffer.from("Hello, World!"));