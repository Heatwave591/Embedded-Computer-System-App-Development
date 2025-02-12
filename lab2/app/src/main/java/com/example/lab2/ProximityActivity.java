package com.example.lab2;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.RadioButton;
import androidx.appcompat.app.AppCompatActivity;

public class ProximityActivity extends AppCompatActivity implements SensorEventListener {

    private SensorManager sensorManager;
    private Sensor proximitySensor;
    private RadioButton[] radioButtons = new RadioButton[7]; // Array for all 7 radio buttons
    private boolean isNear = false; // Flag to track if the sensor detects proximity
    private static final float PROXIMITY_THRESHOLD = 3.0f; // Minimum proximity value to consider as "near"

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_proximity);

        // Initialize all 7 radio buttons
        radioButtons[0] = findViewById(R.id.radio1);
        radioButtons[1] = findViewById(R.id.radio2);
        radioButtons[2] = findViewById(R.id.radio3);
        radioButtons[3] = findViewById(R.id.radio4);
        radioButtons[4] = findViewById(R.id.radio5);
        radioButtons[5] = findViewById(R.id.radio6);
        radioButtons[6] = findViewById(R.id.radio7);

        // Initialize SensorManager and Proximity Sensor
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        if (sensorManager != null) {
            proximitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
            if (proximitySensor != null) {
                sensorManager.registerListener(this, proximitySensor, SensorManager.SENSOR_DELAY_UI);
            }
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_PROXIMITY) {
            // Log the proximity value for debugging
            Log.d("ProximitySensor", "Proximity value: " + event.values[0]);

            // Check if the sensor detects an object close (within range)
            boolean newState = event.values[0] < PROXIMITY_THRESHOLD;

            // Log the new state (near or far)
            Log.d("ProximitySensor", "New state (isNear): " + newState);

            // Only update the state if it has changed from the last state
            if (newState != isNear) {
                isNear = newState;

                // If the sensor detects proximity (covered), light up all radio buttons
                if (isNear) {
                    Log.d("ProximitySensor", "Object is near, turning on all radio buttons.");
                    // Ensure radio buttons are updated on the UI thread
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            for (RadioButton radioButton : radioButtons) {
                                radioButton.setChecked(true); // Set all radio buttons to true
                            }
                        }
                    });
                } else {
                    // If the sensor is not covered, turn off all radio buttons
                    Log.d("ProximitySensor", "Object is far, turning off all radio buttons.");
                    // Ensure radio buttons are updated on the UI thread
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            for (RadioButton radioButton : radioButtons) {
                                radioButton.setChecked(false); // Set all radio buttons to false
                            }
                        }
                    });
                }

                // Log the state of all radio buttons for debugging
                logRadioButtonStates();
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // This method is not needed for this sensor
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Unregister the sensor listener to avoid memory leaks
        if (sensorManager != null) {
            sensorManager.unregisterListener(this);
        }
    }

    // Method to log the state of all radio buttons
    private void logRadioButtonStates() {
        for (int i = 0; i < radioButtons.length; i++) {
            Log.d("RadioButtonState", "RadioButton " + (i + 1) + " is checked: " + radioButtons[i].isChecked());
        }
    }
}
