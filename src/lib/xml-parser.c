#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include "xml-parser.h"

char *get_content_from_attribute(xmlTextReaderPtr reader, const char *attribute) {
    xmlChar *value;
    char *ret;

    /* To read contents between attributes we need its children with "InnerXML" */
    value = xmlTextReaderReadInnerXml(reader);
    if(value != NULL && xmlStrcmp(value, (const xmlChar *) "") != 0) {
        //printf("attrib_name: '%s', value: '%s'\n", attrib_name, value);
        ret = (char *) value;
    } else {
        ret = NULL;
    }
    //xmlFree(value);
    
    return ret;
}

/** handling of a node in the tree
 * add found data to data model
 */
void processNode(SessionHandle *handle, xmlTextReaderPtr reader) {
    xmlChar *attrib_name = xmlTextReaderName(reader);

    if(xmlStrcmp(attrib_name, (const xmlChar *) "serviceType") == 0) {
        char *type = get_content_from_attribute(reader, "serviceType");
        if(type != NULL) {
            handle->service_cnt++;
            //TODO: Add to handler
            Service *service = (Service *) malloc(sizeof(Service));
            service->service_type = get_content_from_attribute(reader, "serviceType");
            printf("SERVICE_TYPE: '%s'\n", service->service_type);
        }
    }
    xmlFree(attrib_name);
    //get_content_from_attribute(reader, "controlURL");
    
    //TODO:
    //Free service + service->service_type
}

void parse_desc(SessionHandle *handle, const char *xmlString) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForDoc((const xmlChar *) xmlString, NULL, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(handle, reader);
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
