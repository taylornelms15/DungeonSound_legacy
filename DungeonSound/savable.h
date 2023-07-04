#ifndef SAVABLE_H
#define SAVABLE_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/**
 * @brief Convenience macro for loading attributes stored under their parameter names
 * @param READER reference to the QXmlStreamReader
 * @param NAME name of attribute/parameter
 * @param TYPE type of parameter; should be uppercase, matching toString, toFloat, etc
 *
 * Example usage: LOAD_ATTRIBUTE(reader, resource_url, String)
 */
#define LOAD_ATTRIBUTE(READER, NAME, TYPE) \
{ \
    if (READER.attributes().hasAttribute(#NAME)) { \
        NAME = READER.attributes().value(#NAME).to##TYPE(); \
    } \
}

#define SAVE_ATTRIBUTE(WRITER, NAME) \
{ \
    WRITER.writeAttribute(#NAME, QString(NAME)); \
}

#define SAVE_ATTRIBUTE_NUMERIC(WRITER, NAME) \
{ \
        WRITER.writeAttribute(#NAME, QString::number(NAME)); \
}

class Savable
{
public:
    /**
     * @brief Saves this element to an output file.
     * @param writer XML-writing object to be used (or passed) to write contents of savable element
     * @return 0 on success, negative value otherwise
     */
    virtual int saveToFile(QXmlStreamWriter& writer) const = 0;

    /**
     * @brief Loads this element from a save file
     * @param reader XML-reading object to be used to read a savable element
     * @return 0 on success, negative value otherwise
     */
    virtual int loadFromFile(QXmlStreamReader& reader) = 0;

    /**
     * @brief Gets the element name string for this savable element
     * @return String used in save file for this savable element
     */
    virtual QString elementName() const = 0;
};

#endif // SAVABLE_H
