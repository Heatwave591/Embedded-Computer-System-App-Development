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

public class AccelerometerActivity extends AppCompatActivity implements SensorEventListener {

    private SensorManager sensorManager;
    private Sensor accelerometerSensor;
    private RadioButton[] radioButtons = new RadioButton[7]; // Array for radio buttons

    // Acceleration thresholds (adjust for better sensitivity)
    private static final float LEVEL_1_THRESHOLD = 30.0f;  // Small shake
    private static final float LEVEL_2_THRESHOLD = 40.0f;  // Medium shake
    private static final float LEVEL_3_THRESHOLD = 50.0f;  // Strong shake
    private static final float LEVEL_4_THRESHOLD = 60.0f;  // Very strong shake
    private static final float LEVEL_5_THRESHOLD = 70.0f;  // Hard shake
    private static final float LEVEL_6_THRESHOLD = 80.0f; // Intense shake
    private static final float LEVEL_7_THRESHOLD = 90.0f; // Maximum shake

    private int lastLevel = 0;  // Stores the last activated level

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_accelerometer);

        // Initialize all 7 radio buttons
        radioButtons[0] = findViewById(R.id.radio1);
        radioButtons[1] = findViewById(R.id.radio2);
        radioButtons[2] = findViewById(R.id.radio3);
        radioButtons[3] = findViewById(R.id.radio4);
        radioButtons[4] = findViewById(R.id.radio5);
        radioButtons[5] = findViewById(R.id.radio6);
        radioButtons[6] = findViewById(R.id.radio7);

        // Ensure all lights are OFF initially
        updateRadioButtons(0);

        // Initialize SensorManager and Accelerometer Sensor
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        if (sensorManager != null) {
            accelerometerSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            if (accelerometerSensor != null) {
                sensorManager.registerListener(this, accelerometerSensor, SensorManager.SENSOR_DELAY_UI);
            } else {
                Log.e("AccelerometerActivity", "Accelerometer sensor not available.");
            }
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];

            // Compute total acceleration ignoring gravity
            float acceleration = (float) Math.sqrt(x * x + y * y + z * z) - SensorManager.GRAVITY_EARTH;
            acceleration = Math.max(acceleration, 0); // Ensure non-negative value

            Log.d("AccelerometerActivity", "Acceleration: " + acceleration);

            int level = calculateLevel(acceleration);

            // **Only update if the level increases (prevents flickering)**
            if (level > lastLevel) {
                lastLevel = level;
                updateRadioButtons(level);
            }

            // **DO NOT RESET LEVEL WHEN SHAKING STOPS**
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // No action needed
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (sensorManager != null) {
            sensorManager.unregisterListener(this);
        }
    }

    /**
     * Determines the number of radio buttons to light up based on acceleration.
     */
    private int calculateLevel(float acceleration) {
        if (acceleration > LEVEL_7_THRESHOLD) return 7;
        else if (acceleration > LEVEL_6_THRESHOLD) return 6;
        else if (acceleration > LEVEL_5_THRESHOLD) return 5;
        else if (acceleration > LEVEL_4_THRESHOLD) return 4;
        else if (acceleration > LEVEL_3_THRESHOLD) return 3;
        else if (acceleration > LEVEL_2_THRESHOLD) return 2;
        else if (acceleration > LEVEL_1_THRESHOLD) return 1;
        else return 0; // **KEEP PREVIOUS LEVEL**
    }

    /**
     * Updates the radio button states based on the detected acceleration level.
     */
    private void updateRadioButtons(int level) {
        runOnUiThread(() -> {
            for (int i = 0; i < radioButtons.length; i++) {
                radioButtons[i].setChecked(i < level);
            }
        });
    }
}