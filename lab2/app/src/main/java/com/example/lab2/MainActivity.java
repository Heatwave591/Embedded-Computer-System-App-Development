package com.example.lab2;
import com.example.lab2.AccelerometerActivity;
import com.example.lab2.ProximityActivity;
import com.example.lab2.GyroscopeActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnAccelerometer = findViewById(R.id.btnAccelerometer);
        Button btnGyroscope = findViewById(R.id.btnGyroscope);
        Button btnProximity = findViewById(R.id.btnProximity);

        // Click Event to Start Loading Activity
        View.OnClickListener startAccelerometerActivity = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, AccelerometerActivity.class);
                startActivity(intent);
            }
        };
        View.OnClickListener startProximityActivity = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, ProximityActivity.class);
                startActivity(intent);
            }
        };
        View.OnClickListener startGyroscope = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, GyroscopeActivity.class);
                startActivity(intent);
            }
        };

        btnAccelerometer.setOnClickListener(startAccelerometerActivity);
        btnGyroscope.setOnClickListener(startGyroscope);
        btnProximity.setOnClickListener(startProximityActivity);
    }
}