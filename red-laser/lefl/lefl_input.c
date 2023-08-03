/*
 * lefl_input.c
 */
#include "lefl.h"

void lefl_key_update(lefl_key_t* key,bool state)
{
    if((!(key->state))&&state)
    {
        key->trigger=true;
        if(key->key_cb!=NULL)
            key->key_cb(key);
    }
    if(!state)
    {
        key->trigger=false;
    }
    key->state=state;
}
bool lefl_key_is_triggered(lefl_key_t* key)
{
    if(key->trigger)
    {
        key->trigger=false;
        return true;
    }
    else
    {
        return false;
    }
}

void lefl_advanced_key_update(lefl_advanced_key_t* advanced_key,float value)
{
    switch(advanced_key->mode)
    {
    case LEFL_KEY_DIGITAL_MODE:
        if((!(advanced_key->key.state))&&value)
        {
            if(advanced_key->key.state==false&&advanced_key->key.key_cb!=NULL)
                advanced_key->key.key_cb(&(advanced_key->key));
            advanced_key->key.trigger=true;
        }
        if(!value)
        {
            advanced_key->key.trigger=false;
        }
        advanced_key->key.state=value;
        break;
    case LEFL_KEY_ANALOG_NORMAL_MODE:
        advanced_key->value=value;
        if(advanced_key->value-advanced_key->schmitt_parameter>advanced_key->trigger_distance)
        {
            if(advanced_key->key.state==false)
            {
                if(advanced_key->key.key_cb!=NULL)
                    advanced_key->key.key_cb(&(advanced_key->key));
                advanced_key->key.trigger=true;
            }
            advanced_key->key.state=true;
        }
        if(advanced_key->value+advanced_key->schmitt_parameter<advanced_key->trigger_distance)
        {
            advanced_key->key.state=false;
            advanced_key->key.trigger=false;
        }
        break;
    case LEFL_KEY_ANALOG_RAPID_MODE:
        advanced_key->value=value;
        if (advanced_key->key.state)
        {
            if (advanced_key->value - advanced_key->minimum - advanced_key->schmitt_parameter >= advanced_key->trigger_distance)
            {
                if (advanced_key->value > advanced_key->maximum)
                    advanced_key->maximum = advanced_key->value;
            }
            if (advanced_key->maximum - advanced_key->value - advanced_key->schmitt_parameter >= advanced_key->release_distance)
            {
                advanced_key->minimum = advanced_key->value;
                advanced_key->key.state = false;
                advanced_key->key.trigger=false;
            }
        }
        else
        {
            if (advanced_key->value - advanced_key->minimum - advanced_key->schmitt_parameter >= advanced_key->trigger_distance)
            {
                advanced_key->maximum = advanced_key->value;
                if(advanced_key->key.state==false)
                {
                    if(advanced_key->key.key_cb!=NULL)
                        advanced_key->key.key_cb(&(advanced_key->key));
                    advanced_key->key.trigger=true;
                }
                advanced_key->key.state = true;
            }
            if (advanced_key->maximum - advanced_key->value - advanced_key->schmitt_parameter >= advanced_key->release_distance)
            {
                if (advanced_key->value < advanced_key->minimum)
                    advanced_key->minimum = advanced_key->value;
            }
        }
        if (advanced_key->value <= 0.0)
        {
            advanced_key->key.state = false;
            advanced_key->key.trigger=false;
            advanced_key->minimum = 0.0;
        }
        if (advanced_key->value >= 1.0)
        {
            if(advanced_key->key.state==false)
            {
                if(advanced_key->key.key_cb!=NULL)
                    advanced_key->key.key_cb(&(advanced_key->key));
                advanced_key->key.trigger=true;
            }
            advanced_key->key.state = true;
            advanced_key->maximum = 1.0;
        }
        break;
    case LEFL_KEY_ANALOG_SPEED_MODE:
        if(value-advanced_key->value > advanced_key->trigger_speed)
        {
            if(advanced_key->key.state==false)
            {
                if(advanced_key->key.key_cb!=NULL)
                    advanced_key->key.key_cb(&(advanced_key->key));
                advanced_key->key.trigger=true;
            }
            advanced_key->key.state=true;
        }
        if(value-advanced_key->value < advanced_key->release_speed)
        {
            advanced_key->key.state=false;
            advanced_key->key.trigger=false;
        }
        advanced_key->value=value;
        break;
    }
}

void lefl_advanced_key_update_raw(lefl_advanced_key_t* key, float value)
{
    key->raw=value;
    if(key->mode==LEFL_KEY_DIGITAL_MODE)
        lefl_advanced_key_update(key,value);
    else
        lefl_advanced_key_update(key,lefl_advanced_key_normalize(key, value));
}

void lefl_advanced_key_update_state(lefl_advanced_key_t* advanced_key, bool state)
{
    if((!(advanced_key->key.state))&&state)
    {
        advanced_key->key.trigger=true;
    }
    if(!state)
    {
        advanced_key->key.trigger=false;
    }
    advanced_key->key.state=state;
}

float lefl_advanced_key_normalize(lefl_advanced_key_t* key, float value)
{
    float x;
    x=(key->upper_bound-(key->upper_bound - key->lower_bound)*key->upper_deadzone -value)/(key->range);
    return x;
}

bool lefl_advanced_key_is_triggered(lefl_advanced_key_t* advanced_key)
{
    if(advanced_key->key.trigger)
    {
        advanced_key->key.trigger=false;
        return true;
    }
    else
    {
        return false;
    }
}

void lefl_advanced_key_set_range(lefl_advanced_key_t* key,float upper,float lower)
{
    key->upper_bound=upper;
    key->lower_bound=lower;
    key->range = key->upper_bound - key->lower_bound;
}

void lefl_advanced_key_set_deadzone(lefl_advanced_key_t* key,float upper,float lower)
{
    /*
    key->upper_deadzone = (key->upper_bound - key->lower_bound)*upper;
    key->lower_deadzone = (key->upper_bound - key->lower_bound)*lower;
    key->range = (key->upper_bound - key->upper_deadzone) - (key->lower_bound+key->lower_deadzone);
    */
    key->upper_deadzone = upper;
    key->lower_deadzone = lower;
    key->range = (key->upper_bound - (key->upper_bound - key->lower_bound)*key->upper_deadzone) - (key->lower_bound+(key->upper_bound - key->lower_bound)*key->lower_deadzone);
}
