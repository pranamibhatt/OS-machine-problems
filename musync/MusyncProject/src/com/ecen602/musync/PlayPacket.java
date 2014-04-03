package com.ecen602.musync;

import java.io.Serializable;

public class PlayPacket implements Serializable{

	/** Auto generated! Do not modify! */
	private static final long serialVersionUID = 3429538255807019677L;

	// Time to start playing
	final long playTime;
	
	// Seek time in msec
	final int offset;
	
	PlayPacket(long playTime, int offset) {
		this.playTime = playTime;
		this.offset = offset;
	}
}
