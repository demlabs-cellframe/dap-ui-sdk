/****************************************//**
 * @page pageServerListAndModels Server List and Models
 *
 * <p>This document describes the behavior and the structure of the *server list* and *models*.</p>
 * <p>All *models* in this document are made with emphasis on work in **QML** environment.</p>
 *
 * @section secListClasses List classes
 *
 * There are two classes of the list of servers:
 * - DapServerList
 * - DapSortedServerList
 *
 * @section secDapServerList DapServerList
 *
 * <p>DapServerList - is a wrapper for DapServerInfoList *vector*, and is also a **QAbstractListModel** *model*.</p>
 * <p>A class copy can be created by the user, but the class also provides a *singleton* option using the **instance** method.</p>
 * <p>In addition to overloading the *model*, the class also provides methods for iterating the list, as well as some operators and the request method from the **QML** environment.</p>
 *
 * @section secDapSortedServerList DapSortedServerList
 *
 * <p>DapSortedServerList – the class encapsulate the functionality of the DapServerList class, which makes them interchangeable (but not dependent on each other). The main feature of the class is to sort list by *ping* and store indexes into **QList**, giving an easy way to sort the elements.</p>
 * <p>Sorting occurs on appending\inserting elements, and can also be performed manually by executing **update** method for the entire list immediately or by executing method with the list of indexes as an argument that requires an update.</p>
 *
 * @section secListModelClasses List model classes
 *
 * Classes of server list models:
 * - DapQmlModelAutoServerList
 * - DapQmlModelFullServerList
 *
 * All presented classes inerhited **QAbstractListModel**.
 *
 * @section secDapQmlModelAutoServerList DapQmlModelAutoServerList
 *
 * <p>DapQmlModelAutoServerList - a *server list model*, which collects average *locations* from the *server list* and withdraw *servers* sorted by *ping*.</p>
 * <p>In other words, it collects *locations*, then adds the most affordable *servers* and withdraws the result.</p>
 * <p>Another feature is to detect the best suitable *server*, name it **Auto** and place it at the beginning of the *list*. It can be the best *server* for *location* provided by *user* or based on the *ping*, if *user location* is not found.</p>
 * <p>The *model* inside constructor is connected to the signals of the *server list* for processing and updating the list of *auto-servers* only when it’s necessary.</p>
 *
 * @section secDapQmlModelFullServerList DapQmlModelFullServerList
 *
 * DapQmlModelFullServerList - combined model. It combines two models in one:
 * - DapQmlModelAutoServerList
 * - DapSortedServerList
 *
 * <p>Both *models* are displayed in one go. The combined model class is connected to both *models signals* for processing upcoming changes, which provides a high level of performance.</p>
 * <p>The *combined model* itself does not control the copies of connected *models*. This is done by the bridge class AbstractServerListModelBridge, which returns these *models* through the request pointer methods.</p>
 * <p>In order to replace standard *models*, you can attach another bridge with the overloaded request methods, which will lead to another list.</p>
 * <p>Also, if there is a need to describe extra fields, there is no need to rewrite all models, since you can describe the processing of those fields in the AbstractServerListModelBridge class by overloading its **customData** and **customRoleNames** methods.</p>
 *
 * @section secAbstractServerListModelBridge AbstractServerListModelBridge
 *
 * <p>AbstractServerListModelBridge - class that provides the opportunity to reload pointers to *server lists* and abstractly attach them to the *full list model* DapQmlModelFullServerList.</p>
 * <p>The class has a default *instance* available from the **instance** method. This instance accesses a *singleton* version of the *sorted list* and attached to it DapQmlModelAutoServerList class.</p>
 * <p>The class also provides a custom fields handling in case the model list fields needs to be expanded. For example if there is need for “favorite” field, but it is not provided by the *server list*, you need to overload the **customData** method and describe the field there according to the analogue with the **QAbstractListModel::data**, as well as overload the **customRoleNames** method by the same analogy with **QAbstractListModel::roleNames**.</p>
 * <p>The names of the fields returned by **customRoleNames** will be processed in the **customData** method, so when writing the handling custom *fields*, do not forget add their description to the roles list.</p>
 *
 *******************************************/
