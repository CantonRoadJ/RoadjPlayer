#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

#include "qmmp/include/tagreadandwrite.h"
#include <QPixmap>

class MusicSongTag
{

public:
    /*!
     * Object contsructor.
     */
    MusicSongTag();
    /*!
     * Object contsructor.
     */
    explicit MusicSongTag(const QString &file);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Read music file to anaylsis.
     */
    bool read();
    /*!
     * Read music file to anaylsis.
     */
    bool read(const QString &file);
    /*!
     * Save music tags to music file.
     */
    bool save();

    /*!
     * Read music decoder name.
     */
    QString getDecoder() const;
    /*!
     * Read music file path.
     */
    QString getFilePath() const;
    /////////////////////////////////////////////
    /*!
     * Get artist name.
     */
    QString getArtist() const;
    /*!
     * Get song title.
     */
    QString getTitle() const;
    /*!
     * Get song album.
     */
    QString getAlbum() const;
    /*!
     * Get song comment.
     */
    QString getComment() const;
    /*!
     * Get song year.
     */
    QString getYear() const;
    /*!
     * Get song track number.
     */
    QString getTrackNum() const;
    /*!
     * Get song genre.
     */
    QString getGenre() const;
    /*!
     * Get song album artist.
     */
    QString getAlbumArtist() const;
    /*!
     * Get song composer.
     */
    QString getComposer() const;
    /*!
     * Get song channel.
     */
    QString getChannel() const;
    /*!
     * Get song file path.
     */
    QString getURL() const;

    /////////////////////////////////////////////
    /*!
     * Set artist name.
     */
    void setArtist(const QString &artist);
    /*!
     * Set song title.
     */
    void setTitle(const QString &title);
    /*!
     * Set song album.
     */
    void setAlbum(const QString &album);
    /*!
     * Set song comment.
     */
    void setComment(const QString &comment);
    /*!
     * Set song year.
     */
    void setYear(const QString &year);
    /*!
     * Set song track number.
     */
    void setTrackNum(const QString &track);
    /*!
     * Set song genre.
     */
    void setGenre(const QString &genre);
    /*!
     * Set song size.
     */
    void setSize(const QString &size);
    /////////////////////////////////////////////
    /*!
     * Set song image cover art.
     */
    void setCover(const QPixmap &pix);
    /*!
     * Set song image cover art.
     */
    void setCover(const QByteArray &data);
    /*!
     * Get song image cover art.
     */
    QPixmap getCover() const;
    /////////////////////////////////////////////
    /*!
     * Get song sample rate.
     */
    QString getSamplingRate() const;
    /*!
     * Get song format.
     */
    QString getFormat() const;
    /*!
     * Get song mode.
     */
    QString getMode() const;
    /*!
     * Get song bitrate.
     */
    QString getBitrate() const;
    /*!
     * Get song time length.
     */
    QString getLengthString() const;

    /*!
     * Get song size length.
     */
    QString getSize() const;

    /////////////////////////////////////////////

protected:
    /*!
     * Find current pluin store path.
     */
    //QString findPluginPath() const;
    /*!
     * Read other taglib not by plugin.
     */
    bool readOtherTaglib();
    /*!
     * Save other taglib not by plugin.
     */
    bool saveOtherTaglib();

    QString m_filePath;
    QMap<TagReadAndWrite::MusicTag, QString> m_parameters;
    QMap<TagReadAndWrite::MusicTag, QPixmap> m_pixMap;
    QString m_fileSize;
};

#endif // MUSICSONGTAG_H
