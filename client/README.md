# client

To install dependencies:

```bash
bun install
```

To run:

```bash
cd client
```

```bash
bun run index.ts
```

This project was created using `bun init` in bun v1.1.12. [Bun](https://bun.sh) is a fast all-in-one JavaScript runtime.

## Client Reading Schema

```json
{
  "clientId": "string",
  "currentReading": 1.0, //float
  "unix": 23, // int unix timestamp
  "fwVersion": "string",
  "unit": "string"
}
```
