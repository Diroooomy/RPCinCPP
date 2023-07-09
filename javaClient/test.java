// import java.util.*;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.Socket;



public class test {

    public class PARAMS {
        public int a;
        public int b;
    }
    public class MSG {
        public char[] funcname = new char[20];
        int ret;
        PARAMS params;
    }

    public static void main(String[] args) {
        String host = "127.0.0.1";
        int port = 16555;
        try {
            Socket client = new Socket(host, port);
            Writer writer = new OutputStreamWriter(client.getOutputStream());
            writer.write("add");
            writer.flush();
            // PARAMS params= new PARAMS();
            // params.a =1;
            // params.b = 2;
            // MSG msg = new MSG();
            // msg.funcname = "add".toCharArray();
            // msg.params = params;
            // msg.ret = 0;




            writer.close();
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}