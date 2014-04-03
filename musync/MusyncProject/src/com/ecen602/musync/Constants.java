package com.ecen602.musync;

public class Constants {
	public static final int PORT = 5000;
	
	public static final int BROADCAST_PORT = 5001;
	
	/**
	 * A multicast group is specified by a class D IP address
	 * and by a standard UDP port number. Class D IP addresses	
	 * are in the range <CODE>224.0.0.0</CODE> to <CODE>239.255.255.255</CODE>,
	 * inclusive. The address 224.0.0.0 is reserved and should not be used.
	 */
	public static final String BROADCAST_IP = "230.1.1.1";
	public static final int BROADCAST_INTERVAL = 5000; // in milliseconds
	
	// hard coded serverip, to be changed later
	public static final String SERVERIP = "192.168.1.3";
}
