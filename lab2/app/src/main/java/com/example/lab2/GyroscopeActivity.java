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

public class GyroscopeActivity extends AppCompatActivity implements SensorEventListener {

    private SensorManager sensorManager;
    private Sensor rotationVectorSensor;
    private RadioButton[] radioButtons = new RadioButton[7]; // Array for all 7 radio buttons

    private static final float LEVEL_1_THRESHOLD = 10.0f;  // Small tilt threshold
    private static final float LEVEL_2_THRESHOLD = 25.0f;  // Medium tilt threshold
    private static final float LEVEL_3_THRESHOLD = 40.0f;  // Large tilt threshold

    private int lastActiveState = 0; // Store last known tilt level (prevents blinking)

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gyroscope);

        // Initialize all 7 radio buttons
        radioButtons[0] = findViewById(R.id.radio1);
        radioButtons[1] = findViewById(R.id.radio2);
        radioButtons[2] = findViewById(R.id.radio3);
        radioButtons[3] = findViewById(R.id.radio4); // Middle (Always ON)
        radioButtons[4] = findViewById(R.id.radio5);
        radioButtons[5] = findViewById(R.id.radio6);
        radioButtons[6] = findViewById(R.id.radio7);

        // Ensure the middle button is always ON
        radioButtons[3].setChecked(true);

        // Initialize SensorManager and Rotation Vector Sensor
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        if (sensorManager != null) {
            rotationVectorSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
            if (rotationVectorSensor != null) {
                sensorManager.registerListener(this, rotationVectorSensor, SensorManager.SENSOR_DELAY_UI);
            } else {
                Log.e("GyroscopeActivity", "Rotation vector sensor not available.");
            }
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
            float[] rotationMatrix = new float[9];
            float[] orientationValues = new float[3];

            // Convert rotation vector into a rotation matrix
            SensorManager.getRotationMatrixFromVector(rotationMatrix, event.values);
            SensorManager.getOrientation(rotationMatrix, orientationValues);

            float roll = (float) Math.toDegrees(orientationValues[2]); // Rotation around X-axis

            Log.d("GyroscopeActivity", "Tilt Angle (Roll): " + roll);

            int newState = 0; // Track new state

            // Determine the tilt level and update the radio buttons
            if (roll < -LEVEL_3_THRESHOLD) {
                newState = -3;
            } else if (roll < -LEVEL_2_THRESHOLD) {
                newState = -2;
            } else if (roll < -LEVEL_1_THRESHOLD) {
                newState = -1;
            } else if (roll > LEVEL_3_THRESHOLD) {
                newState = 3;
            } else if (roll > LEVEL_2_THRESHOLD) {
                newState = 2;
            } else if (roll > LEVEL_1_THRESHOLD) {
                newState = 1;
            } else {
                newState = 0;
            }

            // Update buttons **only if state changes** (prevents blinking)
            if (newState != lastActiveState) {
                lastActiveState = newState;

                if (newState == -3) activateLeftButtons(3);
                else if (newState == -2) activateLeftButtons(2);
                else if (newState == -1) activateLeftButtons(1);
                else if (newState == 3) activateRightButtons(3);
                else if (newState == 2) activateRightButtons(2);
                else if (newState == 1) activateRightButtons(1);
                else resetToMiddle();
            }
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
     * Activates left-side buttons progressively based on the tilt level.
     */
    private void activateLeftButtons(int level) {
        runOnUiThread(() -> {
            clearAllButtonsExceptMiddle();
            if (level >= 1) radioButtons[2].setChecked(true);
            if (level >= 2) radioButtons[1].setChecked(true);
            if (level >= 3) radioButtons[0].setChecked(true);
        });
    }

    /**
     * Activates right-side buttons progressively based on the tilt level.
     */
    private void activateRightButtons(int level) {
        runOnUiThread(() -> {
            clearAllButtonsExceptMiddle();
            if (level >= 1) radioButtons[4].setChecked(true);
            if (level >= 2) radioButtons[5].setChecked(true);
            if (level >= 3) radioButtons[6].setChecked(true);
        });
    }

    /**
     * Resets to the middle button only when the phone is upright.
     */
    private void resetToMiddle() {
        runOnUiThread(this::clearAllButtonsExceptMiddle);
    }

    /**
     * Clears all buttons except for the middle button.
     */
    private void clearAllButtonsExceptMiddle() {
        for (int i = 0; i < radioButtons.length; i++) {
            radioButtons[i].setChecked(i == 3);
        }
    }
}
