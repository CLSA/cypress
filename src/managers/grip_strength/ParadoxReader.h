#pragma once

#include <QString>
#include <QJsonObject>
#include <QFile>
#include <QList>
#include <QTemporaryFile>

typedef QList<QJsonObject> q_paradoxRecords;
typedef QList<q_paradoxRecords> q_paradoxBlocks;

enum class FieldType
{
    Alpha = 0x01,
    Date = 0x02,
    ShortInteger = 0x03,
    LongInteger = 0x04,
    Currency = 0x05,
    Number = 0x06,
    Logical = 0x09,
    MemoBlob = 0x0C,
    BLOB = 0x0D,
    FormatedMemoBlod = 0x0E,
    OLE = 0x0F,
    GraphicBlob = 0x10,
    Time = 0x14,
    Timestamp = 0x15,
    AutoInc = 0x16,
    BCD = 0x17,
    Bytes = 0x18
};

class DbHeaderField
{
    public:
        DbHeaderField(FieldType type, int size): m_type(type), m_size(size) {};

        FieldType getType() const { return m_type; }
        int getSize() const { return m_size; }

        QString getName() const { return m_name; }
        void setName(const QString& name) { m_name = name; }
    private:
        FieldType m_type;
        int m_size;
        QString m_name;
};

class DbHeader
{
    public:
        // Directly from bytes
        qint16 recordSize;
        qint16 headerSize;
        quint8 fileType;
        quint8 maxTableSize;
        qint32 numRecords;
        quint16 nextBlock;
        quint16 fileBlocks;
        quint16 firstBlock;
        quint16 lastBlock;
        quint8 modifiedFlags1;
        quint8 indexFieldNumber;
        qint32 primaryIndexWorkspace;
        qint16 numFields;
        qint16 primaryKeyFields;
        qint32 encryption1;
        quint8 sortOrder;
        quint8 modifiedFlags2;
        quint8 changeCount1;
        quint8 changeCount2;
        qint32 tableNamePtr;
        qint32 fieldInfoPtr;
        quint8 writeProtected;
        quint8 fileVersionId;
        quint16 maxBlocks;
        quint8 auxPassword;
        qint32 cryptInfoStartPtr;
        qint32 cryptInfoEndPtr;
        qint32 autoInc;
        quint8 indexUpdateRequired;
        quint8 refIntegrity;

        // Not directly from bytes
        QList<DbHeaderField> fields;
};

class ParadoxDbBlock {
    public:
        ParadoxDbBlock(qint32 blockNumber, qint32 nextBlock, qint32 prevBlock,
        int offsetToLastRecord, long fileOffset, qint32 addDataSize, DbHeader* header) :
        m_blockNumber(blockNumber),
        m_nextBlock(nextBlock),
        m_prevBlock(prevBlock),
        m_offsetToLastRecord(offsetToLastRecord),
        m_fileOffset(fileOffset),
        m_addDataSize(addDataSize),
        m_dbHeader(header)
        {
        }

        // offsetToLastRecord is set to -header.recordSize when the block is empty.
        // this method will thus return 0 in this case
        int getNumRecords() { return (m_addDataSize / m_dbHeader->recordSize) + 1; }

        QList<QJsonObject> readRecords(QFile& dbFile);

    private:
        qint32 m_blockNumber;
        qint32 m_nextBlock;
        qint32 m_prevBlock;
        qint32 m_offsetToLastRecord;
        qint32 m_fileOffset;
        qint32 m_addDataSize;
        DbHeader* m_dbHeader;

        QJsonObject readRecord(QFile& dbFile);

        // Not using reference for bytes field because a copy
        // is required and do not want to alias
        QVariant getValue(QByteArray bytes, FieldType type);
        QByteArray fixSign(QByteArray bytes);
};

class ParadoxReader
{
    public:
        ParadoxReader(const QString& filePath, QWidget* parent = Q_NULLPTR);
        ~ParadoxReader();

        q_paradoxRecords Read();
        void closeDatabase();

        DbHeader getHeader();
    private:
        void openDatabase(const QString& filePath);

        void backupDatabase();
        void restoreDatabase();

        // Methods for reading in header data
        void readHeader();
        bool headerDataValid();
        void seekToFieldInfoStart();
        void readHeaderFieldInfo();
        void skipPastUneededInfo();
        void readHeaderFieldNames();

        // Read blocks of data
        QList<ParadoxDbBlock> readBlocks();

        // Helper methods for reading various types of data
        qint16 readShort(int offset = -1);
        quint8 readUByte(int offset = -1);
        qint32 readInt(int offset = -1);
        quint16 readUShort(int offset = -1);
        qint32 readPtr(int offset = -1);

        void handleError(QJsonObject* json, const QString& errorMsg);
       // void writeBlockInJson(QJsonObject* json, QList<ParadoxRecord> records, int blockNum);

        QTemporaryFile examDbBackup;
        QTemporaryFile resultsDbBackup;

        QFile m_dbFile;
        DbHeader m_header;
};

