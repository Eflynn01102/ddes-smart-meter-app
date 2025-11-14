import type { BillData } from "./billData";

export interface ServerToClientEvents {
  alert: (message: SocketAlter) => void;
  bill_data: (data: SocketData) => void;
  historical_bill_data: (data: SocketData) => void;
  valid_user: (user: SocketValidUser) => void
  current_usage: (usage: SocketMeter) => void;
}

export interface ClientToServerEvents {
  hello: () => void;
  request_historical_bill_data: (date: string) => void;
  request_user: (user: SocketUnknownUser) => void;
}

export interface InterServerEvents {
  ping: () => void;
}

export interface SocketData {
  accountId: string
  periodStart: string
  currency: string
  energyCost: number
  standingCharge: number
  tax: string
  amountDue: number
}

export interface SocketAlter {
  clientId: string;
  title: string;
  severity: string;
  message: string;
}

export interface SocketValidUser {
  userName: string;
  roles: string;
  clientId: string;
  password: string;
}

export interface SocketUnknownUser {
  userName: string;
  password: string;
}

export interface SocketMeter {
  clientId: string;
  currentUsage: number;
}
