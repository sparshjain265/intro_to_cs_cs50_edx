package com.example.javaexample;

public class House {
    private String name;
    private String head;

    House(String name, String head){
        this.name = name;
        this.head = head;
    }

    public String getName(){
        return name;
    }

    public String getHead(){
        return head;
    }
}
