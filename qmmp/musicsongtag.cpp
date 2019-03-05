#include "musicsongtag.h"
#include "qmmp/tagreadandwrite.h"
#include "qmmp/metadatamanager.h"
#include "qmmp/metadatamodel.h"
#include <QFileInfo>
#include <QBuffer>
#include <QDateTime>
#include <QDebug>

MusicSongTag::MusicSongTag()

{

}

/*MusicSongTag::MusicSongTag(const QString &file)
    : MusicSongTag()
{
    m_filePath = file;
}*/

MusicSongTag::MusicSongTag(const QString &file)
{
    m_filePath = file;
}

QString MusicSongTag::getClassName()
{
    return "MusicSongTag";
}

bool MusicSongTag::read()
{
    if(m_filePath.isEmpty())
    {
        return false;
    }

    return read(m_filePath);
}

bool MusicSongTag::read(const QString &file)
{
    QFile f(file);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    m_filePath = file;
    return readOtherTaglib();
}

bool MusicSongTag::save()
{
    return saveOtherTaglib();
}

//QString MusicSongTag::getDecoder() const
//{
//    QString v = findPluginPath();
//    return QFileInfo(v).baseName();
//}

QString MusicSongTag::getFilePath() const
{
    return m_filePath;
}

QString MusicSongTag::getArtist() const
{
    return m_parameters[TagReadAndWrite::TAG_ARTIST];
}

QString MusicSongTag::getTitle() const
{
    return m_parameters[TagReadAndWrite::TAG_TITLE];
}

QString MusicSongTag::getAlbum() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUM];
}

QString MusicSongTag::getComment() const
{
    return m_parameters[TagReadAndWrite::TAG_COMMENT];
}

QString MusicSongTag::getYear() const
{
    return m_parameters[TagReadAndWrite::TAG_YEAR];
}

QString MusicSongTag::getTrackNum() const
{
    QString v = m_parameters[TagReadAndWrite::TAG_TRACK];
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? "-" : v;
    }
    return "-";
}

QString MusicSongTag::getGenre() const
{
    return m_parameters[TagReadAndWrite::TAG_GENRE];
}

QString MusicSongTag::getAlbumArtist() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUMARTIST];
}

QString MusicSongTag::getComposer() const
{
    return m_parameters[TagReadAndWrite::TAG_COMPOSER];
}

QString MusicSongTag::getChannel() const
{
    return m_parameters[TagReadAndWrite::TAG_CHANNEL];
}

QString MusicSongTag::getURL() const
{
    return m_parameters[TagReadAndWrite::TAG_URL];
}

QString MusicSongTag::getSize() const
{
    return m_fileSize;
}

/////////////////////////////////////////////
void MusicSongTag::setArtist(const QString &artist)
{
    m_parameters[TagReadAndWrite::TAG_ARTIST] = artist;
}

void MusicSongTag::setTitle(const QString &title)
{
    m_parameters[TagReadAndWrite::TAG_TITLE] = title;
}

void MusicSongTag::setAlbum(const QString &album)
{
    m_parameters[TagReadAndWrite::TAG_ALBUM] = album;
}

void MusicSongTag::setComment(const QString &comment)
{
    m_parameters[TagReadAndWrite::TAG_COMMENT] = comment;
}

void MusicSongTag::setYear(const QString &year)
{
    m_parameters[TagReadAndWrite::TAG_YEAR] = year;
}

void MusicSongTag::setTrackNum(const QString &track)
{
    m_parameters[TagReadAndWrite::TAG_TRACK] = track;
}

void MusicSongTag::setGenre(const QString &genre)
{
    m_parameters[TagReadAndWrite::TAG_GENRE] = genre;
}

void MusicSongTag::setSize(const QString &size)
{
    m_fileSize = size;
}

void MusicSongTag::setCover(const QPixmap &pix)
{
    QPixmap p(pix);
    if(p.width() > 500 || p.height() > 500)
    {
        p = p.scaled(500, 500, Qt::KeepAspectRatio);
    }
    m_pixMap[TagReadAndWrite::TAG_COVER] = p;
}

void MusicSongTag::setCover(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    if(pix.width() > 500 || pix.height() > 500)
    {
        pix = pix.scaled(500, 500, Qt::KeepAspectRatio);
    }
    m_pixMap[TagReadAndWrite::TAG_COVER] = pix;
}

QPixmap MusicSongTag::getCover() const
{
    return m_pixMap[TagReadAndWrite::TAG_COVER];
}

/////////////////////////////////////////////
QString MusicSongTag::getSamplingRate() const
{
    return m_parameters[TagReadAndWrite::TAG_SAMPLERATE];
}

QString MusicSongTag::getFormat() const
{
    return m_parameters[TagReadAndWrite::TAG_FORMAT];
}

QString MusicSongTag::getMode() const
{
    return m_parameters[TagReadAndWrite::TAG_MODE];
}

QString MusicSongTag::getBitrate() const
{
    return m_parameters[TagReadAndWrite::TAG_BITRATE];
}

QString MusicSongTag::getLengthString() const
{
    return m_parameters[TagReadAndWrite::TAG_LENGTH];
}

bool MusicSongTag::readOtherTaglib()
{
    TagReadAndWrite tag;
    tag.readFile(m_filePath);
    m_parameters = tag.getMusicTags();
    //    QMap<TagReadAndWrite::MusicTag,QString>::iterator it;
    //        for ( it = m_parameters.begin(); it != m_parameters.end(); ++it )
    //               {
    //                   //用key()和data()分别获取“键”和“值”
    //                   qDebug() << it.key() << it.value();
    //               }
    QString bitRate = getBitrate().split(" ").first();
    int temp = (bitRate.toInt() * getLengthString().toInt());
    temp = temp  / (8 * 1024 * 1024);
    m_fileSize = QString("%1 MB").arg(temp);
    QPixmap cover = MetaDataManager::instance()->getCover(m_filePath);
    if(!cover.isNull())
        setCover(cover);

    return !m_parameters.isEmpty();
}

bool MusicSongTag::saveOtherTaglib()
{

    //            QPixmap pix = getCover();
    //            if(!pix.isNull())
    //            {
    //                QByteArray data;
    //                QBuffer buffer(&data);
    //                if(buffer.open(QIODevice::WriteOnly))
    //                    pix.save(&buffer,"jpg");
    //                buffer.close();
    //                model->setCover(data);
    //            }
    //            ////////////////////////////////////////////////////////////////////
    //        }

    return true;
}
