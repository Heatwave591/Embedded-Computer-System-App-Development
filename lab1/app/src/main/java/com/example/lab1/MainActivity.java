package com.example.lab1;


import android.view.View;
import android.widget.RadioButton;
import android.os.Bundle;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
    private int check = 0;
    private final int st = 8;
    public void countup(View view) {
        check = (check + 1) % st;
        change();
    }

    public void countdn(View view) {
        check = (check - 1 + st) % st;
        change();
    }

    public void clear(View view) {
        check = 0;
        change();
    }

    private void change() {
        RadioButton button0 = (RadioButton) findViewById(R.id.radioButton0);
        RadioButton button1 = (RadioButton) findViewById(R.id.radioButton1);
        RadioButton button2 = (RadioButton) findViewById(R.id.radioButton2);


        button0.setChecked((check & 0b001) != 0);
        button1.setChecked((check & 0b010) != 0);
        button2.setChecked((check & 0b100) != 0);
    }
    }



