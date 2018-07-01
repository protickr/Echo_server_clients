import java.io.*;
import java.net.*;

public class client_java{

    public static void main(String[] args) {
        int args_count = args.length;
        if(args_count < 2){

            System.out.println("Too few arguments exiting");
            System.out.println("enter, <IP> <UserName>");
            System.exit(0);           
        }

        int port = 3210;
        String serv_address = new String(args[0]);
        String user_name = new String(args[1]);
        String user_input = new String("Initialize");
        String resp_buff;
        String send_buff = new String("Initialize");

        try{

            Socket clnt_socket = new Socket(serv_address, port);
            PrintWriter out = new PrintWriter(clnt_socket.getOutputStream());
            BufferedReader in = new BufferedReader(new InputStreamReader(clnt_socket.getInputStream()));
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            
            if(!(clnt_socket.isConnected())){
                //connection could not be created. 
                System.out.println("Connection failed");
                System.exit(1);
            }

            while(true){

                if(user_input.equals("exit")){
                    user_input = "";
                    //out.println(user_input);
                    out.printf("%s", user_input);
                    out.flush();
                    break;
                }

                //out.println(send_buff); //send
                out.printf("%s", send_buff);
                out.flush();

                if((resp_buff = in.readLine()) != null){ //receive //readline discards the \n
                     //looping this condition will wait for 
                    //stream to end/close after it reads the last string's \n
                    System.out.println(resp_buff);
                }

                System.out.printf("%s: ",user_name);
                user_input = stdIn.readLine();
                send_buff = user_name +": "+ user_input;
            }

            in.close();
            out.close();
            clnt_socket.close();
        }catch(IOException e){
            System.out.println(e);
        }
    }
}