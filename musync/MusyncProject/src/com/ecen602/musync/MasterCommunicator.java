package com.ecen602.musync;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import android.util.Log;

public class MasterCommunicator {
	/*
	 * This resource can be claimed if the user has opened the
	 * CreateStationActivity once.
	 */
	private static ServerSocket listenerSocket = null;
	private static MulticastSocket multicastSocket = null;

	boolean listening = true;
	final List<ClientHandler> clients = new ArrayList<ClientHandler>();

	public MasterCommunicator() {

	}

	public void init() {
		Thread listener = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					if (listenerSocket == null)
						listenerSocket = new ServerSocket(Constants.PORT);

					while (listening) {
						Socket socket = listenerSocket.accept();
						if (socket != null) {

							final ClientHandler ch = new ClientHandler(socket);
							ch.setup();
							clients.add(ch);
							Log.w("Musync", "Client Added");
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
				} finally {
					try {
						if (listenerSocket != null)
							listenerSocket.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}, "Server Listener");
		listener.start();

		Thread multicastThread = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					multicastSocket = new MulticastSocket();
					String name = "Musync Server1";
					byte[] buf = name.getBytes();

					while (true) {
						try {
							InetAddress group = InetAddress.getByName(Constants.BROADCAST_IP);
							DatagramPacket packet = 
									new DatagramPacket(buf, buf.length, group, Constants.BROADCAST_PORT);
							multicastSocket.send(packet);

							try {
								Thread.sleep(Constants.BROADCAST_INTERVAL / 2);
							} catch (InterruptedException e) {
							}
						} catch (IOException e) {
							e.printStackTrace();
							break;
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}, "Server Multicast");
		multicastThread.start();
	}

	public void recvTimePacket(TimePacket timePacket) {

	}

	public void sendMessage() {
		Thread sender = new Thread(new Runnable() {
			@Override
			public void run() {
				send(System.currentTimeMillis());
			}
		}, "MasterCommunicator Sender");
		sender.start();
	}

	private void send(long now) {
		List<ClientHandler> remove = new ArrayList<ClientHandler>();

		for (ClientHandler ch : clients) {
			try {
				ch.send(now);
			} catch (IOException e) {
				Log.w("Musync", "Can't send to client. Removing from list");
				remove.add(ch);
				ch.close();
			}
		}

		clients.removeAll(remove);
	}

}
