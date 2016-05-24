import twitter4j.Status;

import java.util.Comparator;
import java.util.Date;

/**
 * Created by vadim on 24.05.16.
 */
public class Tweet {

    private final long id;
    private final long userID;
    private final String content;
    private final Date timestamp;
    private final int favoriteCount;
    private final int retweetCount;
    private final String lang;

    public Tweet(Status status) {
        id = status.getId();
        userID = status.getUser().getId();
        content = status.getText();
        timestamp = status.getCreatedAt();
        favoriteCount = status.getFavoriteCount();
        retweetCount = status.getRetweetCount();
        lang = status.getLang();
    }

    public Tweet(long id, String lang) {
        this.id = id;
        userID = 0;
        content = null;
        timestamp = null;
        favoriteCount = 0;
        retweetCount = 0;
        this.lang = lang;
    }

    @Override
    public boolean equals(Object o) {
        if (o == this)
            return true;
        if (!(o instanceof Tweet))
            return false;
        Tweet t2 = (Tweet) o;
        return id == t2.id;
    }

    @Override
    public int hashCode() {
        return Long.hashCode(id);
    }

    @Override
    public String toString() {
        return "ID " + id + ", created at " + timestamp + ", fav count: " + favoriteCount +
                ", ret count: " + retweetCount + ", content: " + content;
    }

    public long getID() {
        return id;
    }

    public long getUserID() {
        return userID;
    }

    public Date getTimeStamp() {
        return timestamp;
    }

    public int getRetweetCount() {
        return retweetCount;
    }

    public int getFavoriteCount() {
        return favoriteCount;
    }

    public String getLang() {
        return lang;
    }

    static class TimeStampComparator implements Comparator<Tweet> {
        public int compare(Tweet t1, Tweet t2) {
            return t1.getTimeStamp().compareTo(t2.getTimeStamp());
        }
    }
}
