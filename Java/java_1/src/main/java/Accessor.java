import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;

import twitter4j.conf.ConfigurationBuilder;
import twitter4j.*;
/**
 * Created by vadim on 24.05.16.
 */
public class Accessor {

    private static OAuthInfo auth;
    private static Twitter twitter;

    public static void init() throws IOException {
        auth = new OAuthInfo();

        ConfigurationBuilder cb = new ConfigurationBuilder();
        cb.setDebugEnabled(true)
                .setOAuthConsumerKey(auth.getConsumerKey())
                .setOAuthConsumerSecret(auth.getConsumerSecret())
                .setOAuthAccessToken(auth.getAccessKey())
                .setOAuthAccessTokenSecret(auth.getAccessSecret());

        TwitterFactory tf = new TwitterFactory(cb.build());
        twitter = tf.getInstance();
    }

    /**
     * @param query - строка поиска, передаваемая в Twitter4j
     * @param since - дата, с которой начинается поиск (достаточно указать день, месяц и год)
     * @param querySize - количество твитов в результирующей выборке
     *
     * @return контейнер твитов
     */
    public static TweetsContainer<Tweet> search(String query, Date since, int querySize) throws
            IOException, InterruptedException, TwitterException {

        Query searchQuery = makeQuery(query, since);
        TweetsContainer<Tweet> results = new ArrayTweetList<Tweet>();

        long maxID = Long.MAX_VALUE;

        do {
            checkTwitterLimits();

            if (maxID != Long.MAX_VALUE) {
                searchQuery.setMaxId(maxID);
            }

            QueryResult qResult = twitter.search(searchQuery);
            List<Status> tweets = qResult.getTweets();
            for (Status s: tweets) {
                results.add(new Tweet(s));
                maxID = Long.min(maxID, s.getId()) - 1;

                if (results.size() == querySize)
                    return results;
            }

            if (qResult.getCount() == 0)
                break;

        } while (results.size() < querySize);

        return results;
    }

    private static void checkTwitterLimits() throws TwitterException, InterruptedException {
        RateLimitStatus searchTweetsRateLimit = twitter.getRateLimitStatus("search").get("/search/tweets");
        int callsLeft = searchTweetsRateLimit.getRemaining();
        int secondsToSleep = searchTweetsRateLimit.getSecondsUntilReset();
        if (callsLeft == 0) {
            Thread.sleep((secondsToSleep + 5) * 1000);
        }
    }

    private static Query makeQuery(String query, Date since) {
        SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy-MM-dd");
        String sinceStr = dateFormatter.format(since);
        Query searchQuery = new Query(query);
        searchQuery.setSince(sinceStr);
        searchQuery.setCount(100);
        return searchQuery;
    }
}
