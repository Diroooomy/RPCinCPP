//package org.javaClient.Calculator;
import org.javaClient.socket;
import org.json.JSONObject;
import java.io.*;

public class Calculator{
    public static int add(int a, int b) {
        add_PARAMS params = new add_PARAMS(a, b);
        add_MSG msg = new add_MSG("add", params);
        JSONObject m = new JSONObject(msg);
        JSONObject json = null;
        try {
            socket client = new socket();
            client.send(m.toString());
            json = client.recive();
            System.out.println(json);
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return (int) json.get("ret");
    }
    public static int sub(int a, int b) {
        sub_PARAMS params = new sub_PARAMS(a, b);
        sub_MSG msg = new sub_MSG("sub", params);
        JSONObject m = new JSONObject(msg);
        JSONObject json = null;
        try {
            socket client = new socket();
            client.send(m.toString());
            json = client.recive();
            System.out.println(json);
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return (int) json.get("ret");
    }
}
