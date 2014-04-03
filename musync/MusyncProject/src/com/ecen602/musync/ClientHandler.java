package com.ecen602.musync;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OptionalDataException;
import java.net.Socket;

public class ClientHandler {
	final Socket socket;
	ObjectOutputStream output = null;
	ObjectInputStream input = null;

	public ClientHandler(Socket socket) {
		this.socket = socket;
	}

	public void setup() {
		try {
			output = new ObjectOutputStream(socket.getOutputStream());
			input = new ObjectInputStream(socket.getInputStream());
		} catch (IOException e) {
			e.printStackTrace();
		}
		Thread listenerThread = new Thread(new Runnable() {
			@Override
			public void run() {
				Object o;
				try {
					o = input.readObject();
					if (o instanceof TimePacket) {
						TimePacket timePacket = (TimePacket) o;
						timePacket.serverRecv = System.currentTimeMillis();
						timePacket.serverSend = System.currentTimeMillis();
						output.writeObject(timePacket);
						output.flush();
					}
				} catch (OptionalDataException e) {
					e.printStackTrace();
				} catch (ClassNotFoundException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}, "Server Listener: " + socket.toString());
		listenerThread.start();
	}

	public void send(long now) throws IOException {
		if (output == null) 
			throw new IOException("ObjectOutputStream not initialized");
		output.writeObject(new PlayPacket(now, 0));
		output.flush();
	}

	public void close() {
		try {
			output.close();
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
