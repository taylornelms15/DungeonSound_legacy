#ifndef SAVABLE_H
#define SAVABLE_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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
