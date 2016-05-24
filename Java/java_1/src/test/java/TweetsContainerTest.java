import org.junit.Before;
import org.junit.Test;

import java.util.*;

import static org.junit.Assert.*;

/**
 * Created by vadim on 24.05.16.
 */
public class TweetsContainerTest {

    private TweetsContainer tweets;

    @Before
    public void setUp() throws Exception {
        TweetsContainer tweets = new ArrayTweetList();
    }

    @Test
    public void testTweetsContainer() throws Exception {
        assertNotNull(tweets);
        assertEquals(0, tweets.size());

        Tweet tw1 = new Tweet(1, "en");
        Tweet tw2 = new Tweet(2, "en");
        Tweet tw3 = new Tweet(3, "fr");

        tweets.add(tw1);
        tweets.add(tw2);

        assertEquals(2, tweets.size());

        tweets.add(tw3);
        tweets.remove(tw1);

        assertEquals(2, tweets.size());

        tweets.clear();

        assertEquals(0, tweets.size());

        List<Tweet> tweets2 = Arrays.asList(tw1, tw2, tw3);
        tweets.addAll(tweets2);

        assertEquals(3, tweets.size());

        Map<String, Integer> test_counts = tweets.countLangs();

        assertEquals((Integer) 2, test_counts.get("en"));
        assertEquals((Integer) 1, test_counts.get("fr"));

    }
}
