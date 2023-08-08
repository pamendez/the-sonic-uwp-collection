#ifndef INI_H
#define INI_H

class IniParser
{
public:
    enum ItemType {
        INI_ITEM_STRING,
        INI_ITEM_INT,
        INI_ITEM_FLOAT,
        INI_ITEM_BOOL,
        INI_ITEM_COMMENT,
    };

    struct ConfigItem {
        ConfigItem() {
            sprintf(section, "");
            sprintf(key, "");
            sprintf(value, "");
            hasSection = false;
            type       = INI_ITEM_STRING;
        }
        char section[0x20];
        bool hasSection = false;
        char key[0x40];
        char value[0x100];
        byte type = INI_ITEM_STRING;
    };

    IniParser() { memset(items, 0, 0x80 * sizeof(ConfigItem)); }
    IniParser(const char *filename);

    int GetString(const char *section, const char *key, char *dest);
    int GetInteger(const char *section, const char *key, int *dest);
    int GetFloat(const char *section, const char *key, float *dest);
    int GetBool(const char *section, const char *key, bool *dest);
    int SetString(const char *section, const char *key, char *value);
    int SetInteger(const char *section, const char *key, int value);
    int SetFloat(const char *section, const char *key, float value);
    int SetBool(const char *section, const char *key, bool value);
    int SetComment(const char *section, const char *key, const char *comment);
    void Write(const char *filename);

    ConfigItem items[0x80];

    int count = 0;
};
#endif // !INI_H
