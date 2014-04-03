package com.ecen602.musync;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Toast;

public class JoinStationActivity extends Activity {
	SlaveCommunicator slaveCommunicator;
	private ImageButton playButton, pauseButton;
	private ListView listView;
	private ArrayAdapter<String> adapter;

	// UI List Values
	static final List<String> list_values = new ArrayList<String>();
	private static final Map<Integer, String> posMap = new HashMap<Integer, String>();

	String selectedHostIp = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_join_station);

		playButton = (ImageButton) findViewById(R.id.playButton);
		pauseButton = (ImageButton) findViewById(R.id.pauseButton);

		// Get ListView object from XML
		listView = (ListView) findViewById(R.id.station_list);

		// Define a new Adapter
		// First parameter - Context
		// Second parameter - Layout for the row
		// Third parameter - ID of the TextView to which the data is written
		// Forth - the Array of data

		adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, android.R.id.text1,
				list_values);

		// Assign adapter to ListView
		listView.setAdapter(adapter);

		// ListView Item Click Listener
		listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {

				// ListView Clicked item index
				int itemPosition = position;
				selectedHostIp = posMap.get(itemPosition);
				
				// ListView Clicked item value
				String itemValue = (String) listView
						.getItemAtPosition(position);

				// Show Alert
				Toast.makeText(getApplicationContext(),
						"Station Selected: " + itemValue, Toast.LENGTH_LONG)
						.show();
			}

		});

		slaveCommunicator = new SlaveCommunicator(this);
		slaveCommunicator.init();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.join_station, menu);
		return true;
	}

	public void updateListView() {
		list_values.clear();
		posMap.clear();

		int i = 0;
		for (String ip : slaveCommunicator.hostMap.keySet()) {
			list_values.add(slaveCommunicator.hostMap.get(ip).name);
			posMap.put(i, ip);
			++i;
		}
		
		//notify that the model changed
        adapter.notifyDataSetChanged();
	}

	public void onPlayPressed(View v) {
		playButton.setVisibility(View.INVISIBLE);
		pauseButton.setVisibility(View.VISIBLE);
		
		slaveCommunicator.connectToHost();
	}

	public void onPausePressed(View v) {
		playButton.setVisibility(View.VISIBLE);
		pauseButton.setVisibility(View.INVISIBLE);
	}

}
