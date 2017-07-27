package com.incarcloud;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.*;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.List;

@SpringBootApplication
public class App implements CommandLineRunner {
    public static void main(String[] args) {
        SpringApplication.run(App.class, args);
    }

    @Autowired
    private DatabaseConfig _cfg;

    @Autowired
    private WuhanConfig _wuhan;

    public void run(String... args) throws Exception{
        System.out.println("=====begin=====");

        // Loading data
        DataRepo repo = new DataRepo();
        repo.setDataSource(_cfg.DataBase, _cfg.User, _cfg.Password);
        List<LastMark> listMarks = repo.fetchMarks();

        // check bound
        GeoBound wuhan = new GeoBound(_wuhan.left, _wuhan.top, _wuhan.right, _wuhan.bottom);
        for(LastMark mark : listMarks){
            if(mark.isInside(wuhan)){
                System.out.println(mark.SayInCity("Wuhan"));
            }
        }

        System.out.println("=====end=====");
    }


}
