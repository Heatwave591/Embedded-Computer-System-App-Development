package com.example.lab_4;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import androidx.appcompat.app.AppCompatActivity;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.Random;

public class MainActivity extends AppCompatActivity {

    private static final int MAX_QUEUE_SIZE = 50;
    private RadioButton[] radioButtons;
    private Button btnHigher, btnLower;

    private BlockingQueue<Integer> queue;

    private Thread producerThread;
    private Thread consumerThread;

    private volatile boolean running = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize queue
        queue = new LinkedBlockingQueue<>(MAX_QUEUE_SIZE);

        initializeUI();
        createAndStartThreads();
    }

    private void initializeUI() {
        radioButtons = new RadioButton[MAX_QUEUE_SIZE];
        for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
            int id = getResources().getIdentifier("radioButton" + (i + 1), "id", getPackageName());
            radioButtons[i] = findViewById(id);
        }

        btnHigher = findViewById(R.id.button3); // "HIGHER" button
        btnLower = findViewById(R.id.button);   // "LOWER" button

        btnHigher.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                increaseProducerPriority();
            }
        });
        btnLower.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                decreaseProducerPriority();
            }
        });
    }

    private void createAndStartThreads() {
        producerThread = new Thread(new Runnable() {
            @Override
            public void run() {
                Random random = new Random();
                while (running) {
                    try {
                        performHeavyComputation();
                        int randomNum = random.nextInt(10);
                        queue.put(randomNum);

                        updateUI();

                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }, "ProducerThread");

        consumerThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (running) {
                    try {
                        performHeavyComputation();

                        Integer value = queue.take();
                        updateUI();

                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        }, "ConsumerThread");

        producerThread.setPriority(Thread.NORM_PRIORITY);
        consumerThread.setPriority(Thread.NORM_PRIORITY);

        producerThread.start();
        consumerThread.start();
    }

    private void performHeavyComputation() {
        // For CPU (Android Studio emulator)
         int iterations = 200000;

        // For Mobile
        // int iterations = 50000;

        double result = 0;
        for (int i = 0; i < iterations; i++) {
            result += Math.sqrt(i) * Math.sin(i) * Math.cos(i);
        }
    }

    private void updateUI() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                int queueSize = queue.size();

                for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
                    radioButtons[i].setChecked(i < queueSize);
                }
            }
        });
    }

    private void increaseProducerPriority() {
        int currentPriority = producerThread.getPriority();
        if (currentPriority < Thread.MAX_PRIORITY) {
            System.out.println(currentPriority);
            producerThread.setPriority(currentPriority + 1);
            // Log priority change - useful for debugging
            System.out.println("Producer thread priority increased to: " + producerThread.getPriority());
        }
    }
    private void decreaseProducerPriority() {
        int currentPriority = producerThread.getPriority();
        if (currentPriority > Thread.MIN_PRIORITY) {
            producerThread.setPriority(currentPriority - 1);
            System.out.println("Producer thread priority decreased to: " + producerThread.getPriority());
        }
    }

    @Override
    protected void onDestroy() {
        running = false;

        if (producerThread != null) {
            producerThread.interrupt();
        }
        if (consumerThread != null) {
            consumerThread.interrupt();
        }

        super.onDestroy();
    }
}