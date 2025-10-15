export interface ServerToClientEvents {
	data: (data: SocketData) => void;
}

export interface ClientToServerEvents {
	hello: () => void;
}

export interface InterServerEvents {
	ping: () => void;
}

export interface SocketData {
	data: number;
}
