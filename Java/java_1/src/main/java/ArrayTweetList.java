import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by vadim on 24.05.16.
 */
public class ArrayTweetList<T extends Tweet> implements TweetsContainer<T> {

    private final List<T> tweets;

    public ArrayTweetList() {
        tweets = new ArrayList<T>();
    }

    public int size() {
        return tweets.size();
    }

    public boolean add(T tweet) {
        return tweets.add(tweet);
    }

    public boolean addAll(Collection<? extends T> collection) {
        return tweets.addAll(collection);
    }

    public boolean remove(T tweet) {
        return tweets.remove(tweet);
    }

    public void clear() {
        tweets.clear();
    }

    public Iterator<T> iterator() {
        return tweets.iterator();
    }

    public Map<String, Integer> countLangs() {
        return tweets.stream().collect(Collectors.groupingBy(T::getLang))
                .entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, e -> e.getValue().size()));
    }
}
