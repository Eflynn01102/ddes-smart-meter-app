import { BillData } from "./billData";

export interface ServerToClientEvents {
  // data: (data: SocketData) => void;
  alert: (message: SocketAlter) => void;
  bill_data: (data: SocketData) => void;
}

export interface ClientToServerEvents {
  hello: () => void;
}

export interface InterServerEvents {
  ping: () => void;
}

export interface SocketData {
  clientId: string;
  data: BillData;
}

export interface SocketAlter {
  clientId: string;
  message: string;
}
