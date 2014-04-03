package com.ecen602.musync;

import android.content.Context;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnErrorListener;
import android.widget.Toast;

public class Player {
	final Context context;
	final MediaPlayer mediaPlayer;

	// Synchronization Parameters in nanosec
	@SuppressWarnings("unused")
	private long delay;
	private long offset;

	Player(final Context context) {
		this.context = context;
		mediaPlayer = MediaPlayer.create(context, R.raw.sample_song);
		
		mediaPlayer.setOnErrorListener(new OnErrorListener() {
			
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra) {
				Toast.makeText(context, "Error in playing media", Toast.LENGTH_LONG).show();
				return false;
			}
		});
	}

	public void play() {
		mediaPlayer.start();
	}

	public void play(final long startTime, final int seek) {
		Thread playbackSchedulerThread = new Thread() {
			public void run() {
				long now = System.currentTimeMillis();
				long scheduled = startTime - offset;

				int seekTime = seek;
				/* Already missed out on some time. Compensate with offset */
				if (scheduled < now) {
					seekTime += (int) (now - scheduled);
				}

				/*
				 * Sleep the thread if there is a long delay. Else run an active
				 * loop
				 */
				while (scheduled > now) {
					long diff = scheduled - now;
					if (diff > 2000) {
						try {
							Thread.sleep(diff - 1500);
						} catch (InterruptedException e) {
						}
					}
					now = System.currentTimeMillis();
				}
				try {
					mediaPlayer.seekTo(seekTime);
					mediaPlayer.start();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		 playbackSchedulerThread.start();
	}

	public void pause() {
		mediaPlayer.pause();
	}

	public void stop() {
		mediaPlayer.stop();
	}

	public void setSyncParams(long delay, long offset) {
		this.delay = delay;
		this.offset = offset;
	}
}
