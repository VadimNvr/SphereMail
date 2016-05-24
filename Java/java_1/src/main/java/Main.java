import java.io.PrintWriter;
import java.util.Calendar;
import java.util.Date;
import java.util.Map;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.stage.Stage;

/**
 * Created by vadim on 24.05.16.
 */
public class Main {
    public static void main(String[] args) {
        Calendar calendar = Calendar.getInstance();
        calendar.clear();
        calendar.set(2016, Calendar.APRIL, 23, 0, 0);
        Date date = calendar.getTime();

        try {
            PrintWriter realWriter = new PrintWriter("Real.csv");
            PrintWriter atleticoWriter = new PrintWriter("Atletico.csv");

            Accessor.init();

            TweetsContainer<Tweet> realMadridTweets = Accessor.search("RealMadrid", date, 5000);
            TweetsContainer<Tweet> atleticoMadridTweets = Accessor.search("AtleticoMadrid", date, 5000);

            Map<String, Integer> realMadridCounts = realMadridTweets.countLangs();
            for (Map.Entry<String, Integer> l: realMadridCounts.entrySet()) {
                realWriter.format("%s;%d\n", l.getKey(), l.getValue());
            }

            Map<String, Integer> atleticoMadridCounts = atleticoMadridTweets.countLangs();
            for (Map.Entry<String, Integer> l: atleticoMadridCounts.entrySet()) {
                atleticoWriter.format("%s;%d\n", l.getKey(), l.getValue());
            }

            realWriter.close();
            atleticoWriter.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
