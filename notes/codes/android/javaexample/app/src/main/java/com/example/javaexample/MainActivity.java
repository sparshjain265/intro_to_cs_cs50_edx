package com.example.javaexample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Trace;
import android.util.Log;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        List<House> houses = new ArrayList<>();
        houses.add(new House("Gryffindor", "McGonagall"));
        houses.add(new House("Hufflepuff", "Sprout"));
        houses.add(new House("Ravenclaw", "Flitwick"));
        houses.add(new House("Slytherin", "Snape"));

        List<String> students = Arrays.asList("Harry", "Ron", "Hermione", "Neville", "Draco", "Parvati", "Padma", "Cho", "Cedric");
        Map<String, House> assignments = new HashMap<>();

        Random random = new Random();
        for (String student : students) {
            int index = random.nextInt(houses.size());
            assignments.put(student, houses.get(index));
        }

        for (Map.Entry<String, House> entry : assignments.entrySet()) {
            House house = entry.getValue();
            Log.d("cs50", entry.getKey() + " got " + house.getName() + " under " + house.getHead());
        }
    }
}