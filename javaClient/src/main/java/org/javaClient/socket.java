package org.javaClient;

import org.json.JSONObject;

import java.io.*;
import java.net.Socket;

public class socket {

    String host = "192.168.214.133";
    int port = 16556;

    Socket client;

    public socket() throws IOException {
        this.client = new Socket(host, port);
    }
    public socket(String host, int port) throws IOException {
        this.client = new Socket(host, port);
    }
    public void send(String msg) throws IOException {
        Writer writer = new OutputStreamWriter(client.getOutputStream());
        writer.write(msg);
        writer.flush();
    }

    public JSONObject recive() throws IOException {
        InputStream ips = client.getInputStream();
        InputStreamReader ipsr = new InputStreamReader(ips);
        BufferedReader br = new BufferedReader(ipsr);
        String ret = br.readLine();
        JSONObject json = new JSONObject(ret);
        return json;
    }

    public void close() throws IOException {
        this.client.close();
    }
}
