//package org.javaClient.Calculator;
import org.javaClient.socket;
import org.json.JSONObject;
import java.io.*;

public class age_sum{
    public static int age_sum(user user1, user user2) {
        age_sum_PARAMS params = new age_sum_PARAMS(user1, user2);
        age_sum_MSG msg = new age_sum_MSG("age_sum", params);
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
