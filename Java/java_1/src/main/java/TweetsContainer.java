import java.io.Serializable;
import java.util.Collection;
import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 * Created by vadim on 24.05.16.
 */
public interface TweetsContainer<T extends Tweet> extends Iterable<T>, Serializable {

    /**
     * Returns the count of tweets in container.
     *
     * @return the count of tweets in container
     */
    int size();

    /**
     * Adds tweet to container
     *
     * @param tweet - tweet to add
     * @return true - if the container changed
     */
    boolean add(T tweet);

    /**
     * Adds a tweet collection to container
     *
     * @param tweets - input tweet collection
     * @return true - if the container changed
     */
    boolean addAll(Collection<? extends T> tweets);

    /**
     * Removes tweet from container, if exists
     *
     * @param tweet - tweet to be removed
     * @return true if the tweet was removed
     */
    boolean remove(T tweet);

    /**
     * Removes all tweets from the container
     */
    void clear();

    /**
     * Counts tweet existances for every language
     *
     * @return map - language -> tweet count
     */
    Map<String, Integer> countLangs();
}
