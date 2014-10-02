#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libxml/xmlreader.h>
#include "xml-parser.h"

void rtrim(char *str) {
    size_t n;
    n = strlen(str);
    while (n > 0 && isspace((unsigned char)str[n - 1])) {
        n--;
    }
    str[n] = '\0';
}

/**
 * Only for debugging: print service_list from handle
 */
void print_service_list(SessionHandle *handle) {
    Service* cur_service = handle->service_list;
    while(cur_service != NULL) {
        if(cur_service->service_type != NULL) {
            printf("SERVICE_TYPE: '%s'\n", cur_service->service_type);
        }
        if(cur_service->control_url != NULL) {
            printf("\tCONTROL_URL: '%s'\n", cur_service->control_url);
        }
        if(cur_service->scpd_url != NULL) {
            printf("\tSCPD_URL: '%s'\n", cur_service->scpd_url);
        }
        cur_service = cur_service->next;
    }
}

char *get_content_from_attribute(xmlTextReaderPtr reader) {
    xmlChar* value;
    char* ret;

    /* To read contents between attributes we need its children with "InnerXML" */
    value = xmlTextReaderReadInnerXml(reader);
    if(value != NULL && xmlStrcmp(value, (const xmlChar *) "") != 0) {
        ret = (char*) value;
    } else {
        free(value);
        ret = NULL;
    }
    
    return ret;
}

void add_service_to_handle(SessionHandle *handle, Service *service) {
    Service *cur_service = handle->service_list;
    if(cur_service == NULL) {
        handle->service_list = service;
    } else {
        while(cur_service->next != NULL) {
            cur_service = cur_service->next;
        }
        cur_service->next = service;
    }
}

Service *get_service_by_type(SessionHandle *handle, const char *service_type) {
    Service* cur_service = handle->service_list;
    Service* ret_service = NULL;
    while(cur_service != NULL) {
        if(strcmp(cur_service->service_type, service_type) == 0) {
            ret_service = cur_service;
            /* stop after the first found */
            break;
        }
        cur_service = cur_service->next;
    }

    return ret_service;
}

void add_value_to_service(SessionHandle *handle, xmlTextReaderPtr reader, Service *service, const char *attribute, char *current_service_type) {
    char *value = NULL;
    value = get_content_from_attribute(reader);
    if(value != NULL) {
        service = get_service_by_type(handle, current_service_type);
        if(service != NULL) {
            if(strcmp(attribute, "controlURL") == 0)
                service->control_url = value;
            if(strcmp(attribute, "SCPDURL") == 0)
                service->scpd_url = value;
        } else {
            free(value);
            fprintf(stderr, "could not find '%s' service type '%s'\n", attribute, current_service_type);
        }
    }
}

/** 
 * handling of a node in the tree
 * add found data to data model
 */
void processNode(SessionHandle *handle, xmlTextReaderPtr reader, char *current_service_type, size_t current_size_type_len) {
    xmlChar *attrib_name = xmlTextReaderName(reader);

    Service *service = NULL;
    char *value = NULL;

    if(xmlStrcmp(attrib_name, (const xmlChar *) "serviceType") == 0) {
        value = get_content_from_attribute(reader);
        if(value != NULL) {
            service = (Service *) malloc(sizeof(Service));
            if(service != NULL) {
                service->service_type = value;
                service->control_url = NULL;
                service->scpd_url = NULL;
                service->next = NULL;
                add_service_to_handle(handle, service);
                strncpy(current_service_type, value, current_size_type_len);
            } else {
                fprintf(stderr, "could not allocate memory!\n");
            }
        }
    }

    else if(xmlStrcmp(attrib_name, (const xmlChar *) "controlURL") == 0) {
        add_value_to_service(handle, reader, service, "controlURL", current_service_type);
    }

    else if(xmlStrcmp(attrib_name, (const xmlChar *) "SCPDURL") == 0) {
        add_value_to_service(handle, reader, service, "SCPDURL", current_service_type);
    }

    xmlFree(attrib_name);
}

int parse_desc(SessionHandle *handle, char *xmlString) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    xmlTextReaderPtr reader;
    int ret;
    /* 
     * we have to remember the current service type 
     * service_type is like an unique ID 
     */
    size_t current_size_type_len = 256;
    char current_service_type[current_size_type_len];

    reader = xmlReaderForDoc((const xmlChar *) xmlString, NULL, NULL, 0);
    if(reader != NULL) {
        if(xmlTextReaderNormalization(reader) != 1) {
            fprintf(stderr, "error during normalization\n");
            return -1;
        }
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(handle, reader, current_service_type, current_size_type_len);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "libxml2: failed to parse XML string\n");
            return -1;
        }
    } else {
        fprintf(stderr, "an error occoured\n");
        return -1;
    }

    /* for debugging */
    print_service_list(handle);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();

    return 0;
}

void get_services(SessionHandle *handle, char *xmlString) {
    /* TODO: handle return value, for errors n stuff */
    parse_desc(handle, xmlString);
}
