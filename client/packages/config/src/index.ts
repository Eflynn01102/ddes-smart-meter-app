export interface ServerToClientEvents {
  data: (data: SocketData) => void;
  alert: (message: SocketAlter) => void;
}

export interface ClientToServerEvents {
  hello: () => void;
}

export interface InterServerEvents {
  ping: () => void;
}

export interface SocketData {
  clientId: string;
  data: number;
}

export interface SocketAlter {
  clientId: string;
  message: string;
}