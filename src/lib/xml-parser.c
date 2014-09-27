#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include "xml-parser.h"

/* handling of a node in the tree */
void processNode(xmlTextReaderPtr reader) {
    xmlChar *attrib_name, *value;
    attrib_name = xmlTextReaderName(reader);

    if(xmlStrcmp(attrib_name, (const xmlChar *) "SCPDURL") == 0) {
        /* To read contents between attributes we need its childs with "InnerXML" */
        value = xmlTextReaderReadInnerXml(reader);
        if(value != NULL && xmlStrcmp(value, (const xmlChar *) "") != 0) {
            printf("SCPDURL: attrib_name: '%s', value: '%s'\n", attrib_name, value);
        }
        xmlFree(value);
    }
    xmlFree(attrib_name);
}

void parse_desc(SessionHandle *handle,const char *xmlString) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* TODO Get Services and write into SessionHandle->services array */
    
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForDoc((const xmlChar *) xmlString, NULL, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "libxml2: failed to parse XML string\n");
        }
    } else {
        fprintf(stderr, "an error occoured\n");
    }

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
}

void get_services(SessionHandle *handle, const char *xmlString) {
    parse_desc(handle, xmlString);
}
