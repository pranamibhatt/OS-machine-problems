package com.ecen602.musync;

import java.io.Serializable;

public class TimePacket implements Serializable {

	/** Auto generated: Do not modify !*/
	private static final long serialVersionUID = 5309321869942414583L;

	long clientSend; // t1
	long serverRecv; // t2
	long serverSend; // t3
	long clientRecv; // t4
}
