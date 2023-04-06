#include "ParadoxReader.h"
#include <QJsonDocument>

ParadoxReader::ParadoxReader(const QString& filePath, QWidget* parent)
{
    Q_UNUSED(parent)
    // Open Database connection
    openDatabase(filePath);
}

ParadoxReader::~ParadoxReader()
{
    closeDatabase();
}


DbHeader ParadoxReader::getHeader()
{
    return m_header;
}


void ParadoxReader::backupDatabase()
{

}

void ParadoxReader::restoreDatabase()
{

}

/*
* Reads in the data stored in a paradox database
*/
q_paradoxRecords ParadoxReader::Read()
{
    // seek to start of database
    m_dbFile.seek(0);

    // Read the header data
    bool errorReadingHeader = false;
    try {
        readHeader();
    }
    catch(...){
        errorReadingHeader = true;
    }

    // Report error if header could not be read or if header data is not valid
    if (errorReadingHeader || !headerDataValid()) {
        qDebug() << "Error: Valid header data could not be read";
        return q_paradoxRecords();
    }

    // Read the actual data
    QList<ParadoxDbBlock> blocks = readBlocks();

    // Convert to Json Object
    // Return error if there is no data found
    if (blocks.isEmpty()) {
        qDebug() << "Error: No data stored in database blocks";
        return q_paradoxRecords();
    }

    q_paradoxRecords records;
    for (int i = 0; i < blocks.count(); i++) {
        q_paradoxRecords _records = blocks[i].readRecords(m_dbFile);
        records.append(_records);
    }

    // Close Database connection
    closeDatabase();
    qDebug() << "Databases read in";

    return records;
}

/*
* Open connection to paradox database
*/
void ParadoxReader::openDatabase(const QString& filePath)
{
    // Open data base
    m_dbFile.setFileName(filePath);
    m_dbFile.open(QFile::ReadOnly);
}

/*
* Close connection with paradox database if it is currently open
*/
void ParadoxReader::closeDatabase()
{
    // Close ZGripTest.DB
    if (m_dbFile.isOpen())
    {
        m_dbFile.close();
    }
}

/*
* Read in the header data from the paradox database
*/
void ParadoxReader::readHeader()
{
    m_header.recordSize = readShort(0x0000);
    m_header.headerSize = readShort(0x0002);
    m_header.fileType = readUByte(0x0004);
    m_header.maxTableSize = readUByte(0x0005);
    m_header.numRecords = readInt(0x0006);
    m_header.nextBlock = readUShort(0x000A);
    m_header.fileBlocks = readUShort(0x000C);
    m_header.firstBlock = readUShort(0x000E);
    m_header.lastBlock = readUShort(0x0010);
    m_header.modifiedFlags1 = readUByte(0x0014);
    m_header.indexFieldNumber = readUByte(0x0015);
    m_header.primaryIndexWorkspace = readPtr(0x0016);
    m_header.numFields = readShort(0x0021);
    m_header.primaryKeyFields = readShort(0x0023);
    m_header.encryption1 = readInt(0x0025);
    m_header.sortOrder = readUByte(0x0029);
    m_header.modifiedFlags2 = readUByte(0x002A);
    m_header.changeCount1 = readUByte(0x002D);
    m_header.changeCount2 = readUByte(0x002E);
    m_header.tableNamePtr = readPtr(0x0030);
    m_header.fieldInfoPtr = readPtr(0x0034);
    m_header.writeProtected = readUByte(0x0038);
    m_header.fileVersionId = readUByte(0x0039);
    m_header.maxBlocks = readUShort(0x003A);
    m_header.auxPassword = readUByte(0x003D);
    m_header.cryptInfoStartPtr = readPtr(0x0040);
    m_header.cryptInfoEndPtr = readPtr(0x0044);
    m_header.autoInc = readInt(0x0049);
    m_header.indexUpdateRequired = readUByte(0x004F);
    m_header.refIntegrity = readUByte(0x0055);

    seekToFieldInfoStart();
    readHeaderFieldInfo();
    skipPastUneededInfo();
    readHeaderFieldNames();
}

bool ParadoxReader::headerDataValid()
{
    // TODO: fill this in
    return true;
}

QJsonObject ParadoxDbBlock::readRecord(QFile& dbFile)
{
        QJsonObject jsonRecord;
        for (int i = 0; i < m_dbHeader->numFields; i++) {
            DbHeaderField field = m_dbHeader->fields[i];
            FieldType type = field.getType();
            QByteArray bytes = dbFile.read(field.getSize());
            QVariant value = getValue(bytes, type);
            qDebug() << field.getName() << " " << value;
            jsonRecord.insert(field.getName(), QJsonValue::fromVariant(value));
        }
        return jsonRecord;
}

QList<QJsonObject> ParadoxDbBlock::readRecords(QFile& dbFile)
{
        int numRecords = getNumRecords();
        qDebug() << numRecords;
        QList<QJsonObject> records;
        dbFile.seek(m_fileOffset);
        for (int i = 0; i < numRecords; i++) {
            QJsonObject record = readRecord(dbFile);
            records.append(record);
        }
        return records;
}

void printVal(qint32 val) {
    qDebug() << QString::number(val, 2).rightJustified(32, '0');
}

QVariant ParadoxDbBlock::getValue(QByteArray bytes, FieldType type) {
        QVariant output;

        switch (type) {
        case FieldType::Alpha:
        {
            QString result;
            result.reserve(bytes.size());
            for (int i = 0; i < bytes.size(); i++) {
                if ((char) bytes[i] != 0) {
                    result.append((char)bytes[i]);
                }
            }
            output = result;
            break;
        }
        case FieldType::LongInteger:
        {
            qint32 value = NULL;
            unsigned char sign = (bytes[0] & 0x80) ? (bytes[0] & 0x7f) : (bytes[0] | 0x80);

            if (bytes[0] & 0x80) { // positive
                value  = (qint32) ((unsigned char) sign << 24);
                value |= (qint32) ((unsigned char) bytes[1] << 16);
                value |= (qint32) ((unsigned char) bytes[2] << 8);
                value |= (qint32) ((unsigned char) bytes[3]);
            }
            else if (bytes[0] & 0x7F) { // negative
                value  = (qint32) ((unsigned char) sign << 24);
                value |= (qint32) ((unsigned char) bytes[1] << 16);
                value |= (qint32) ((unsigned char) bytes[2] << 8);
                value |= (qint32) ((unsigned char) bytes[3]);
            }

            output = value;
            break;
        }
        // These two are in the data set, but ONYX has them return null
        // So likely not needed to use the data from these types
        case FieldType::Logical:
            output = (bytes[0] & 0x81) == 0x81 ? true : false;
            break;
        case FieldType::MemoBlob:
            break;
        default:
            break;
        }
        return output;
    }


QByteArray ParadoxDbBlock::fixSign(QByteArray bytes) {
        if (bytes[0] & 0x80) {
            //bytes[0] &= 0x7f; This is the java code. not too sure about &=
            // Flip the most significant bit to 0 (remove sign bit)
            bytes[0] = bytes[0] & 0x7f;
        }
        return bytes;
    }
/*
* Seek to the start of the header field info stored in the paradox database
* Expects that the dbFile will be set to the end of the refIntegrity
*/
void ParadoxReader::seekToFieldInfoStart()
{
    // If header version is greater than 4...
    if (m_header.fileVersionId > 0x04) {
        // Seek to the start of the field info
        m_dbFile.seek(0x0078);
    }
    // otherwise the field info should start after the refIntegrity
}

/*
* Reads in the header field info stored in the paradox database
* Expects the dbFile to be set to the start of the fieldInfo
*/
void ParadoxReader::readHeaderFieldInfo()
{
    for (int i = 0; i < m_header.numFields; i++) {
        QByteArray bytes = m_dbFile.read(1);
        quint8 type = (quint8)bytes[0];

        bytes = m_dbFile.read(1);
        quint8 size = (quint8)bytes[0];

        DbHeaderField headerField(static_cast<FieldType>(type), size);
        m_header.fields.append(headerField);
    }
}

/*
* Skips past data that does not need to be read in yet
* Expects that the dbFile will be set to the end of the field info
*/
void ParadoxReader::skipPastUneededInfo()
{
    // Skip tableNamePtr
    m_dbFile.skip(4);

    // skipFieldnamePtrArray
    int numFields = (int)(m_header.numFields);
    m_dbFile.skip(4 * numFields);

    // Skip tableName
    m_dbFile.skip(m_header.fileVersionId > 0x04 ? 261 : 79);
}

/*
* Reads in the header field names stored in the paradox database
* Expects the dbFile to be set to the start of the field names
*/
void ParadoxReader::readHeaderFieldNames()
{
    for (int i = 0; i < m_header.numFields; i++) {
        QString fieldname;
        quint8 next = (quint8)m_dbFile.read(1)[0];
        while (next > 0) {
            char nextChar = (char)next;
            fieldname = fieldname + nextChar;
            next = (quint8)m_dbFile.read(1)[0];
        }
        if (next < 0) {
            qDebug() << "BIG PROBLEM. Neg number";
        }
        m_header.fields[i].setName(fieldname);
        qDebug() << i << ". " << "fieldName: " << fieldname;
    }
}

/*
* Read in blocks of data from paradox database
*/
QList<ParadoxDbBlock> ParadoxReader::readBlocks()
{
    long curFileOffset = m_header.headerSize;
    m_dbFile.seek(curFileOffset);
    QList<ParadoxDbBlock> blocks;
    blocks.reserve(m_header.fileBlocks);
    for (int i = 0; i < m_header.fileBlocks; i++) {
        quint16 nextBlock = readUShort(curFileOffset);
        quint16 prevBlock = readUShort(curFileOffset + 2);
        quint16 addDataSize = readUShort(curFileOffset + 4);
        curFileOffset += 2 * 3; // 2 byte read 3 times
        ParadoxDbBlock block(i + 1, nextBlock, prevBlock, 0, curFileOffset, addDataSize, &m_header);
        blocks.append(block);
    }

    return blocks;
}

/*
* Read a short from the paradox database
* Seeks to the offset entered or reads from the current location if
* an offset of -1 is passed in
*/
qint16 ParadoxReader::readShort(int offset)
{
    if (-1 != offset)
        m_dbFile.seek(offset);
    QByteArray bytes = m_dbFile.read(2);
    qint16 value = (qint16)(((qint16)bytes[1]) << 8 | bytes[0]);
    return value;
}

/*
* Read an unsigned byte from the paradox database
* Seeks to the offset entered or reads from the current location if
* an offset of -1 is passed in
*/
quint8 ParadoxReader::readUByte(int offset)
{
    if (-1 != offset)
        m_dbFile.seek(offset);
    QByteArray bytes = m_dbFile.read(1);
    quint8 value = (quint8)bytes[0];
    return value;
}

/*
* Read an integer from the paradox database
* Seeks to the offset entered or reads from the current location if
* an offset of -1 is passed in
*/
qint32 ParadoxReader::readInt(int offset)
{
    if (-1 != offset)
        m_dbFile.seek(offset);
    QByteArray bytes = m_dbFile.read(4);
    qint32 value = (qint32)(((qint32)bytes[3]) << 24 | ((qint32)bytes[2]) << 16 | ((qint16) bytes[1]) << 8 | (qint8) bytes[0]);

    return value;
}

/*
* Read an unsigned short from the paradox database
* Seeks to the offset entered or reads from the current location if
* an offset of -1 is passed in
*/
quint16 ParadoxReader::readUShort(int offset)
{
    if (-1 != offset)
        m_dbFile.seek(offset);
    QByteArray bytes = m_dbFile.read(2);

    quint16 value = (quint16) (((quint16) bytes[1]) << 8) | ((quint8) bytes[0]);

    return value;
}

/*
* Read a pointer from the paradox database
* Seeks to the offset entered or reads from the current location if
* an offset of -1 is passed in
*/
qint32 ParadoxReader::readPtr(int offset)
{
    return readInt(offset);
}

/*
* Adds error message and error occured flag to json object
* Also closes database connection
*/
void ParadoxReader::handleError(QJsonObject* json, const QString& errorMsg)
{
    json->insert("ErrorOccured", true);
    json->insert("ErrorMessage", errorMsg);
    qDebug() << errorMsg;
    closeDatabase();
}

//void ParadoxReader::writeBlockInJson(QJsonObject* json, QList<ParadoxRecord> records, int blockNum)
//{
//    for (int i = 0; i < records.count(); i++) {
//        ParadoxRecord record = records[i];
//        QVariantList values = record.getValues
//        for(int j = 0; j < )
//        QString headerName = m_header.fields[i].getName();
//        QString key = QString("block/%1/%2").arg(blockNum).arg(headerName);
//        json->insert(key, );
//    }
//}
