# Qt Core

## Описание

Qt Core — основной модуль Qt, предоставляющий фундаментальные классы без графики. Содержит систему сигналов/слотов, систему свойств, контейнеры, работу с файлами и потоками.

**Назначение:** Базовая функциональность для всех приложений Qt.

## Обзор

- [Qt Core Overview](https://doc.qt.io/qt-6/qtcore-index.html)
- [Qt Core Module](https://doc.qt.io/qt-6/qtcore-module.html)

## Ключевые концепции

### Система сигналов и слотов
- [Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html)
- [Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

### Система свойств
- [The Property System](https://doc.qt.io/qt-6/properties.html)

### Объектная модель
- [Object Model](https://doc.qt.io/qt-6/object.html)
- [Object Trees & Ownership](https://doc.qt.io/qt-6/objecttrees.html)

## Многопоточность

- [Threading and Concurrent Programming](https://doc.qt.io/qt-6/threads.html)
- [QThread Documentation](https://doc.qt.io/qt-6/qthread.html)
- [Qt Concurrent Module](https://doc.qt.io/qt-6/qtconcurrent-index.html)

## Контейнеры

- [Container Classes](https://doc.qt.io/qt-6/containers.html)
- [Implicit Sharing](https://doc.qt.io/qt-6/implicit-sharing.html)

## Примеры

- [Qt Core Examples](https://doc.qt.io/qt-6/qtcore-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(my_target PRIVATE Qt6::Core)
```

## Ключевые классы

**Актуальный список всех классов модуля:** [Qt Core C++ Classes](https://doc.qt.io/qt-6/qtcore-module.html)


| Класс | Описание | Ссылка |
|-------|---------|--------|
| `Qt` | Core C++ Classes \| | [Qt](https://doc.qt.io/qt-6/qt.html) |
| `QQXBTC` | '); | [QQXBTC](https://doc.qt.io/qt-6/qqxbtc.html) |
| `QtContains` | miscellaneous identifiers used throughout the | [QtContains](https://doc.qt.io/qt-6/qtcontains.html) |
| `Qt::LiteralsInline` | namespace declares literal operators for | [Qt::LiteralsInline](https://doc.qt.io/qt-6/qt::literalsinline.html) |
| `Qt::Literals::StringLiteralsDeclares` | literal operators for | [Qt::Literals::StringLiteralsDeclares](https://doc.qt.io/qt-6/qt::literals::stringliteralsdeclares.html) |
| `QtFutureContains` | miscellaneous identifiers used by the | [QtFutureContains](https://doc.qt.io/qt-6/qtfuturecontains.html) |
| `QFuture` | class | [QFuture](https://doc.qt.io/qt-6/qfuture.html) |
| `QtJniTypesDeclares` | C++ types that correspond to Java types | [QtJniTypesDeclares](https://doc.qt.io/qt-6/qtjnitypesdeclares.html) |
| `QtLiteralsDeclares` | literal operators for | [QtLiteralsDeclares](https://doc.qt.io/qt-6/qtliteralsdeclares.html) |
| `QAbstractAnimationThe` | base of all animations | [QAbstractAnimationThe](https://doc.qt.io/qt-6/qabstractanimationthe.html) |
| `QAbstractEventDispatcherInterface` | to manage | [QAbstractEventDispatcherInterface](https://doc.qt.io/qt-6/qabstracteventdispatcherinterface.html) |
| `QAbstractItemModelThe` | abstract interface for item model classes | [QAbstractItemModelThe](https://doc.qt.io/qt-6/qabstractitemmodelthe.html) |
| `QAbstractListModelAbstract` | model that can be subclassed to create one-dimensional list models | [QAbstractListModelAbstract](https://doc.qt.io/qt-6/qabstractlistmodelabstract.html) |
| `QAbstractNativeEventFilterInterface` | for receiving native events, such as MSG or XCB event structs | [QAbstractNativeEventFilterInterface](https://doc.qt.io/qt-6/qabstractnativeeventfilterinterface.html) |
| `QAbstractProxyModelBase` | class for proxy item models that can do sorting, filtering or other data processing tasks | [QAbstractProxyModelBase](https://doc.qt.io/qt-6/qabstractproxymodelbase.html) |
| `QAbstractTableModelAbstract` | model that can be subclassed to create table models | [QAbstractTableModelAbstract](https://doc.qt.io/qt-6/qabstracttablemodelabstract.html) |        
| `QAdoptSharedDataTagHelper` | tag class | [QAdoptSharedDataTagHelper](https://doc.qt.io/qt-6/qadoptshareddatataghelper.html) |
| `QAnimationGroupAbstract` | base class for groups of animations | [QAnimationGroupAbstract](https://doc.qt.io/qt-6/qanimationgroupabstract.html) |
| `QAnyStringViewUnified` | view on Latin-1, UTF-8, or UTF-16 strings with a read-only subset of the | [QAnyStringViewUnified](https://doc.qt.io/qt-6/qanystringviewunified.html) |     
| `QString` | API | [QString](https://doc.qt.io/qt-6/qstring.html) |
| `QAssociativeIterableIterable` | interface for an associative container in a | [QAssociativeIterableIterable](https://doc.qt.io/qt-6/qassociativeiterableiterable.html) |
| `QAtomicIntPlatform` | -independent atomic operations on int | [QAtomicIntPlatform](https://doc.qt.io/qt-6/qatomicintplatform.html) |
| `QAtomicIntegerPlatform` | -independent atomic operations on integers | [QAtomicIntegerPlatform](https://doc.qt.io/qt-6/qatomicintegerplatform.html) |
| `QAtomicPointerTemplate` | class that provides platform-independent atomic operations on pointers | [QAtomicPointerTemplate](https://doc.qt.io/qt-6/qatomicpointertemplate.html) |    
| `QAtomicScopedValueRollbackProvides` | a | [QAtomicScopedValueRollbackProvides](https://doc.qt.io/qt-6/qatomicscopedvaluerollbackprovides.html) |
| `QScopedValueRollback` | for atomic variables | [QScopedValueRollback](https://doc.qt.io/qt-6/qscopedvaluerollback.html) |
| `QBEIntegerPlatform` | -independent big-endian integers | [QBEIntegerPlatform](https://doc.qt.io/qt-6/qbeintegerplatform.html) |
| `QBasicTimerTimer` | events for objects | [QBasicTimerTimer](https://doc.qt.io/qt-6/qbasictimertimer.html) |
| `QBindableWrapper` | class around binding-enabled properties. It allows type-safe operations while abstracting the differences between the various property classes away | [QBindableWrapper](https://doc.qt.io/qt-6/qbindablewrapper.html) |
| `QBitArrayArray` | of bits | [QBitArrayArray](https://doc.qt.io/qt-6/qbitarrayarray.html) |
| `QBluetoothPermissionAccess` | Bluetooth peripherals | [QBluetoothPermissionAccess](https://doc.qt.io/qt-6/qbluetoothpermissionaccess.html) |
| `QBufferQIODevice` | interface for a | [QBufferQIODevice](https://doc.qt.io/qt-6/qbufferqiodevice.html) |
| `QByteArrayArray` | of bytes | [QByteArrayArray](https://doc.qt.io/qt-6/qbytearrayarray.html) |
| `QByteArray::FromBase64ResultQByteArray::FromBase64Result` | class holds the result of a call to | [QByteArray::FromBase64ResultQByteArray::FromBase64Result](https://doc.qt.io/qt-6/qbytearray::frombase64resultqbytearray::frombase64result.html) |
| `QByteArrayListList` | of byte arrays | [QByteArrayListList](https://doc.qt.io/qt-6/qbytearraylistlist.html) |
| `QByteArrayMatcherHolds` | a sequence of bytes that can be quickly matched in a byte array | [QByteArrayMatcherHolds](https://doc.qt.io/qt-6/qbytearraymatcherholds.html) |
| `QByteArrayViewView` | on an array of bytes with a read-only subset of the | [QByteArrayViewView](https://doc.qt.io/qt-6/qbytearrayviewview.html) |
| `QCacheTemplate` | class that provides a cache | [QCacheTemplate](https://doc.qt.io/qt-6/qcachetemplate.html) |
| `QCalendarDescribes` | calendar systems | [QCalendarDescribes](https://doc.qt.io/qt-6/qcalendardescribes.html) |
| `QCalendarPermissionAccess` | the user's calendar | [QCalendarPermissionAccess](https://doc.qt.io/qt-6/qcalendarpermissionaccess.html) |
| `QCameraPermissionAccess` | the camera for taking pictures or videos | [QCameraPermissionAccess](https://doc.qt.io/qt-6/qcamerapermissionaccess.html) |
| `QCborArrayUsed` | to hold an array of CBOR elements | [QCborArrayUsed](https://doc.qt.io/qt-6/qcborarrayused.html) |
| `QCborErrorHolds` | the error condition found while parsing or validating a CBOR stream | [QCborErrorHolds](https://doc.qt.io/qt-6/qcborerrorholds.html) |
| `QCborMapUsed` | to hold an associative container representable in CBOR | [QCborMapUsed](https://doc.qt.io/qt-6/qcbormapused.html) |
| `QCborMap::ConstIteratorQCborMap::ConstIterator` | class provides an STL-style const iterator for | [QCborMap::ConstIteratorQCborMap::ConstIterator](https://doc.qt.io/qt-6/qcbormap::constiteratorqcbormap::constiterator.html) |
| `QCborMap::IteratorQCborMap::Iterator` | class provides an STL-style non-const iterator for | [QCborMap::IteratorQCborMap::Iterator](https://doc.qt.io/qt-6/qcbormap::iteratorqcbormap::iterator.html) |
| `QCborParserErrorUsed` | by | [QCborParserErrorUsed](https://doc.qt.io/qt-6/qcborparsererrorused.html) |
| `QCborValue` | to report a parsing error | [QCborValue](https://doc.qt.io/qt-6/qcborvalue.html) |
| `QCborStreamReaderSimple` | CBOR stream decoder, operating on either a | [QCborStreamReaderSimple](https://doc.qt.io/qt-6/qcborstreamreadersimple.html) |
| `QCborStreamWriterSimple` | CBOR encoder operating on a one-way stream | [QCborStreamWriterSimple](https://doc.qt.io/qt-6/qcborstreamwritersimple.html) |
| `QCborValueEncapsulates` | a value in CBOR | [QCborValueEncapsulates](https://doc.qt.io/qt-6/qcborvalueencapsulates.html) |
| `QChar16` | -bit Unicode character | [QChar16](https://doc.qt.io/qt-6/qchar16.html) |     
| `QChildEventContains` | event parameters for child object events | [QChildEventContains](https://doc.qt.io/qt-6/qchildeventcontains.html) |
| `QChronoTimerRepetitive` | and single-shot timers | [QChronoTimerRepetitive](https://doc.qt.io/qt-6/qchronotimerrepetitive.html) |
| `QCollatorCompares` | strings according to a localized collation algorithm | [QCollatorCompares](https://doc.qt.io/qt-6/qcollatorcompares.html) |
| `QCollatorSortKeyCan` | be used to speed up string collation | [QCollatorSortKeyCan](https://doc.qt.io/qt-6/qcollatorsortkeycan.html) |
| `QCommandLineOptionDefines` | a possible command-line option | [QCommandLineOptionDefines](https://doc.qt.io/qt-6/qcommandlineoptiondefines.html) |
| `QCommandLineParserMeans` | for handling the command line options | [QCommandLineParserMeans](https://doc.qt.io/qt-6/qcommandlineparsermeans.html) |
| `QConcatenateTablesProxyModelProxies` | multiple source models, concatenating their rows | [QConcatenateTablesProxyModelProxies](https://doc.qt.io/qt-6/qconcatenatetablesproxymodelproxies.html) |
| `QConstIteratorAllows` | iteration over a container in a | [QConstIteratorAllows](https://doc.qt.io/qt-6/qconstiteratorallows.html) |
| `QContactsPermissionAccess` | the user's contacts | [QContactsPermissionAccess](https://doc.qt.io/qt-6/qcontactspermissionaccess.html) |
| `QContiguousCacheTemplate` | class that provides a contiguous cache | [QContiguousCacheTemplate](https://doc.qt.io/qt-6/qcontiguouscachetemplate.html) |
| `QCoreApplicationEvent` | loop for | [QCoreApplicationEvent](https://doc.qt.io/qt-6/qcoreapplicationevent.html) |
| `QCryptographicHashWay` | to generate cryptographic hashes | [QCryptographicHashWay](https://doc.qt.io/qt-6/qcryptographichashway.html) |
| `QDataStreamSerialization` | of binary data to a | [QDataStreamSerialization](https://doc.qt.io/qt-6/qdatastreamserialization.html) |
| `QDateDate` | functions | [QDateDate](https://doc.qt.io/qt-6/qdatedate.html) |
| `QDateTimeDate` | and time functions | [QDateTimeDate](https://doc.qt.io/qt-6/qdatetimedate.html) |
| `QDeadlineTimerMarks` | a deadline in the future | [QDeadlineTimerMarks](https://doc.qt.io/qt-6/qdeadlinetimermarks.html) |
| `QDebugOutput` | stream for debugging information | [QDebugOutput](https://doc.qt.io/qt-6/qdebugoutput.html) |
| `QDebugStateSaverConvenience` | class for custom | [QDebugStateSaverConvenience](https://doc.qt.io/qt-6/qdebugstatesaverconvenience.html) |
| `QDebug` | operators | [QDebug](https://doc.qt.io/qt-6/qdebug.html) |
| `QDirAccess` | to directory structures and their contents | [QDirAccess](https://doc.qt.io/qt-6/qdiraccess.html) |
| `QDirIteratorIterator` | for directory entrylists | [QDirIteratorIterator](https://doc.qt.io/qt-6/qdiriteratoriterator.html) |
| `QDirListingSTL` | -style iterator for directory entries | [QDirListingSTL](https://doc.qt.io/qt-6/qdirlistingstl.html) |
| `QDynamicPropertyChangeEventContains` | event parameters for dynamic property change events | [QDynamicPropertyChangeEventContains](https://doc.qt.io/qt-6/qdynamicpropertychangeeventcontains.html) |
| `QEasingCurveEasing` | curves for controlling animation | [QEasingCurveEasing](https://doc.qt.io/qt-6/qeasingcurveeasing.html) |
| `QElapsedTimerFast` | way to calculate elapsed times | [QElapsedTimerFast](https://doc.qt.io/qt-6/qelapsedtimerfast.html) |
| `QEnableSharedFromThisA` | base class that allows obtaining a | [QEnableSharedFromThisA](https://doc.qt.io/qt-6/qenablesharedfromthisa.html) |
| `QSharedPointer` | for an object already managed by a shared pointer | [QSharedPointer](https://doc.qt.io/qt-6/qsharedpointer.html) |
| `QEventThe` | base class of all event classes. Event objects contain event parameters | [QEventThe](https://doc.qt.io/qt-6/qeventthe.html) |
| `QEventLoopMeans` | of entering and leaving an event loop | [QEventLoopMeans](https://doc.qt.io/qt-6/qeventloopmeans.html) |
| `QEventLoopLockerMeans` | to quit an event loop when it is no longer needed | [QEventLoopLockerMeans](https://doc.qt.io/qt-6/qeventlooplockermeans.html) |
| `QExceptionBase` | class for exceptions that can be transferred across threads | [QExceptionBase](https://doc.qt.io/qt-6/qexceptionbase.html) |
| `QExplicitlySharedDataPointerRepresents` | a pointer to an explicitly shared object | [QExplicitlySharedDataPointerRepresents](https://doc.qt.io/qt-6/qexplicitlyshareddatapointerrepresents.html) |
| `QFileInterface` | for reading from and writing to files | [QFileInterface](https://doc.qt.io/qt-6/qfileinterface.html) |
| `QFileDeviceInterface` | for reading from and writing to open files | [QFileDeviceInterface](https://doc.qt.io/qt-6/qfiledeviceinterface.html) |
| `QFileInfoOS` | -independent API to retrieve information about file system entries | [QFileInfoOS](https://doc.qt.io/qt-6/qfileinfoos.html) |
| `QFileSelectorConvenient` | way of selecting file variants | [QFileSelectorConvenient](https://doc.qt.io/qt-6/qfileselectorconvenient.html) |
| `QFileSystemWatcherInterface` | for monitoring files and directories for modifications | [QFileSystemWatcherInterface](https://doc.qt.io/qt-6/qfilesystemwatcherinterface.html) |     
| `QFlagHelper` | data type for | [QFlagHelper](https://doc.qt.io/qt-6/qflaghelper.html) |  
| `QFlagsType` | -safe way of storing OR-combinations of enum values | [QFlagsType](https://doc.qt.io/qt-6/qflagstype.html) |
| `QFutureRepresents` | the result of an asynchronous computation | [QFutureRepresents](https://doc.qt.io/qt-6/qfuturerepresents.html) |
| `QFutureIteratorJava` | -style const iterator for | [QFutureIteratorJava](https://doc.qt.io/qt-6/qfutureiteratorjava.html) |
| `QFutureSynchronizerConvenience` | class that simplifies | [QFutureSynchronizerConvenience](https://doc.qt.io/qt-6/qfuturesynchronizerconvenience.html) |
| `QFutureWatcherAllows` | monitoring a | [QFutureWatcherAllows](https://doc.qt.io/qt-6/qfuturewatcherallows.html) |
| `QGenericArgumentInternal` | helper class for marshalling arguments | [QGenericArgumentInternal](https://doc.qt.io/qt-6/qgenericargumentinternal.html) |
| `QGenericReturnArgumentInternal` | helper class for marshalling arguments | [QGenericReturnArgumentInternal](https://doc.qt.io/qt-6/qgenericreturnargumentinternal.html) |
| `QGlobalStaticUsed` | to implement a global static object | [QGlobalStaticUsed](https://doc.qt.io/qt-6/qglobalstaticused.html) |
| `QHashTemplate` | class that provides a hash-table-based dictionary | [QHashTemplate](https://doc.qt.io/qt-6/qhashtemplate.html) |
| `QHash::TryEmplaceResultUsed` | to represent the result of a tryEmplace() operation | [QHash::TryEmplaceResultUsed](https://doc.qt.io/qt-6/qhash::tryemplaceresultused.html) |        
| `QHashIteratorJava` | -style const iterator for | [QHashIteratorJava](https://doc.qt.io/qt-6/qhashiteratorjava.html) |
| `QHash` | and | [QHash](https://doc.qt.io/qt-6/qhash.html) |
| `QIODeviceThe` | base interface class of all I/O devices in | [QIODeviceThe](https://doc.qt.io/qt-6/qiodevicethe.html) |
| `QIODeviceBaseBase` | class for | [QIODeviceBaseBase](https://doc.qt.io/qt-6/qiodevicebasebase.html) |
| `QIdentityProxyModelProxies` | its source model unmodified | [QIdentityProxyModelProxies](https://doc.qt.io/qt-6/qidentityproxymodelproxies.html) |
| `QItemSelectionManages` | information about selected items in a model | [QItemSelectionManages](https://doc.qt.io/qt-6/qitemselectionmanages.html) |
| `QItemSelectionModelKeeps` | track of a view's selected items | [QItemSelectionModelKeeps](https://doc.qt.io/qt-6/qitemselectionmodelkeeps.html) |
| `QItemSelectionRangeManages` | information about a range of selected items in a model | [QItemSelectionRangeManages](https://doc.qt.io/qt-6/qitemselectionrangemanages.html) |        
| `QIterableTemplate` | class that is the base class for | [QIterableTemplate](https://doc.qt.io/qt-6/qiterabletemplate.html) |
| `QSequentialIterable` | and | [QSequentialIterable](https://doc.qt.io/qt-6/qsequentialiterable.html) |
| `QIteratorTemplate` | class that allows iteration over a container in a | [QIteratorTemplate](https://doc.qt.io/qt-6/qiteratortemplate.html) |
| `QJniArrayTemplate` | class that represents an array in Java | [QJniArrayTemplate](https://doc.qt.io/qt-6/qjniarraytemplate.html) |
| `QJniArrayBaseCommon` | , type-independent APIs for | [QJniArrayBaseCommon](https://doc.qt.io/qt-6/qjniarraybasecommon.html) |
| `QJniEnvironmentAccess` | to the JNI Environment (JNIEnv) | [QJniEnvironmentAccess](https://doc.qt.io/qt-6/qjnienvironmentaccess.html) |
| `QJniObjectA` | convenience wrapper around the Java Native Interface (JNI) | [QJniObjectA](https://doc.qt.io/qt-6/qjniobjecta.html) |
| `QJsonArrayEncapsulates` | a JSON array | [QJsonArrayEncapsulates](https://doc.qt.io/qt-6/qjsonarrayencapsulates.html) |
| `QJsonDocumentWay` | to read and write JSON documents | [QJsonDocumentWay](https://doc.qt.io/qt-6/qjsondocumentway.html) |
| `QJsonObjectEncapsulates` | a JSON object | [QJsonObjectEncapsulates](https://doc.qt.io/qt-6/qjsonobjectencapsulates.html) |
| `QJsonParseErrorUsed` | to report errors during JSON parsing | [QJsonParseErrorUsed](https://doc.qt.io/qt-6/qjsonparseerrorused.html) |
| `QJsonValueEncapsulates` | a value in JSON | [QJsonValueEncapsulates](https://doc.qt.io/qt-6/qjsonvalueencapsulates.html) |
| `QKeyCombinationStores` | a combination of a key with optional modifiers | [QKeyCombinationStores](https://doc.qt.io/qt-6/qkeycombinationstores.html) |
| `QKeyValueIteratorIterator` | over the key/value pairs of an associative container | [QKeyValueIteratorIterator](https://doc.qt.io/qt-6/qkeyvalueiteratoriterator.html) |
| `QLEIntegerPlatform` | -independent little-endian integers | [QLEIntegerPlatform](https://doc.qt.io/qt-6/qleintegerplatform.html) |
| `QLatin1Char8` | -bit ASCII/Latin-1 character | [QLatin1Char8](https://doc.qt.io/qt-6/qlatin1char8.html) |
| `QLatin1StringThe` | same as | [QLatin1StringThe](https://doc.qt.io/qt-6/qlatin1stringthe.html) |
| `QLatin1StringMatcherOptimized` | search for substring in Latin-1 text | [QLatin1StringMatcherOptimized](https://doc.qt.io/qt-6/qlatin1stringmatcheroptimized.html) |
| `QLatin1StringViewThin` | wrapper around a US-ASCII/Latin-1 encoded string literal | [QLatin1StringViewThin](https://doc.qt.io/qt-6/qlatin1stringviewthin.html) |
| `QLibraryLoads` | shared libraries at runtime | [QLibraryLoads](https://doc.qt.io/qt-6/qlibraryloads.html) |
| `QLibraryInfoInformation` | about the | [QLibraryInfoInformation](https://doc.qt.io/qt-6/qlibraryinfoinformation.html) |
| `QLineTwo` | -dimensional vector using integer precision | [QLineTwo](https://doc.qt.io/qt-6/qlinetwo.html) |
| `QLineFTwo` | -dimensional vector using floating point precision | [QLineFTwo](https://doc.qt.io/qt-6/qlineftwo.html) |
| `QListTemplate` | class that provides a dynamic array | [QListTemplate](https://doc.qt.io/qt-6/qlisttemplate.html) |
| `QListIteratorJava` | -style const iterator for | [QListIteratorJava](https://doc.qt.io/qt-6/qlistiteratorjava.html) |
| `QList` | and | [QList](https://doc.qt.io/qt-6/qlist.html) |
| `QLocaleConverts` | between numbers and their string representations in various languages | [QLocaleConverts](https://doc.qt.io/qt-6/qlocaleconverts.html) |
| `QLocationPermissionAccess` | the user's location | [QLocationPermissionAccess](https://doc.qt.io/qt-6/qlocationpermissionaccess.html) |
| `QLockFileLocking` | between processes using a file | [QLockFileLocking](https://doc.qt.io/qt-6/qlockfilelocking.html) |
| `QLoggingCategoryRepresents` | a category, or 'area' in the logging infrastructure | [QLoggingCategoryRepresents](https://doc.qt.io/qt-6/qloggingcategoryrepresents.html) |
| `QMapTemplate` | class that provides an associative array | [QMapTemplate](https://doc.qt.io/qt-6/qmaptemplate.html) |
| `QMapIteratorJava` | -style const iterator for | [QMapIteratorJava](https://doc.qt.io/qt-6/qmapiteratorjava.html) |
| `QMarginsDefines` | the four margins of a rectangle | [QMarginsDefines](https://doc.qt.io/qt-6/qmarginsdefines.html) |
| `QMarginsFDefines` | the four margins of a rectangle | [QMarginsFDefines](https://doc.qt.io/qt-6/qmarginsfdefines.html) |
| `QMessageAuthenticationCodeWay` | to generate hash-based message authentication codes | [QMessageAuthenticationCodeWay](https://doc.qt.io/qt-6/qmessageauthenticationcodeway.html) |  
| `QMessageLogContextAdditional` | information about a log message | [QMessageLogContextAdditional](https://doc.qt.io/qt-6/qmessagelogcontextadditional.html) |
| `QMessageLoggerGenerates` | log messages | [QMessageLoggerGenerates](https://doc.qt.io/qt-6/qmessageloggergenerates.html) |
| `QMetaAssociationAllows` | type erased access to associative containers | [QMetaAssociationAllows](https://doc.qt.io/qt-6/qmetaassociationallows.html) |
| `QMetaClassInfoAdditional` | information about a class | [QMetaClassInfoAdditional](https://doc.qt.io/qt-6/qmetaclassinfoadditional.html) |
| `QMetaContainerCommon` | functionality for sequential and associative containers | [QMetaContainerCommon](https://doc.qt.io/qt-6/qmetacontainercommon.html) |
| `QMetaEnumMeta` | -data about an enumerator | [QMetaEnumMeta](https://doc.qt.io/qt-6/qmetaenummeta.html) |
| `QMetaMethodMeta` | -data about a member function | [QMetaMethodMeta](https://doc.qt.io/qt-6/qmetamethodmeta.html) |
| `QMetaObjectContains` | meta-information about | [QMetaObjectContains](https://doc.qt.io/qt-6/qmetaobjectcontains.html) |
| `QMetaPropertyMeta` | -data about a property | [QMetaPropertyMeta](https://doc.qt.io/qt-6/qmetapropertymeta.html) |
| `QMetaSequenceAllows` | type erased access to sequential containers | [QMetaSequenceAllows](https://doc.qt.io/qt-6/qmetasequenceallows.html) |
| `QMetaTypeManages` | named types in the meta-object system | [QMetaTypeManages](https://doc.qt.io/qt-6/qmetatypemanages.html) |
| `QMicrophonePermissionAccess` | the microphone for monitoring or recording sound | [QMicrophonePermissionAccess](https://doc.qt.io/qt-6/qmicrophonepermissionaccess.html) |
| `QMimeDataContainer` | for data that records information about its MIME type | [QMimeDataContainer](https://doc.qt.io/qt-6/qmimedatacontainer.html) |
| `QMimeDatabaseMaintains` | a database of MIME types | [QMimeDatabaseMaintains](https://doc.qt.io/qt-6/qmimedatabasemaintains.html) |
| `QMimeTypeDescribes` | types of file or data, represented by a MIME type string | [QMimeTypeDescribes](https://doc.qt.io/qt-6/qmimetypedescribes.html) |
| `QModelIndexUsed` | to locate data in a data model | [QModelIndexUsed](https://doc.qt.io/qt-6/qmodelindexused.html) |
| `QModelRoleDataHolds` | a role and the data associated to that role | [QModelRoleDataHolds](https://doc.qt.io/qt-6/qmodelroledataholds.html) |
| `QModelRoleDataSpanSpan` | over | [QModelRoleDataSpanSpan](https://doc.qt.io/qt-6/qmodelroledataspanspan.html) |
| `QModelRoleData` | objects | [QModelRoleData](https://doc.qt.io/qt-6/qmodelroledata.html) |
| `QMultiHashMulti` | -valued hash table | [QMultiHashMulti](https://doc.qt.io/qt-6/qmultihashmulti.html) |
| `QMultiMapTemplate` | class that provides an associative array with multiple equivalent keys | [QMultiMapTemplate](https://doc.qt.io/qt-6/qmultimaptemplate.html) |
| `QMultiMap::key_iteratorQMultiMap::key_iterator` | class provides an STL-style const iterator for | [QMultiMap::key_iteratorQMultiMap::key_iterator](https://doc.qt.io/qt-6/qmultimap::key_iteratorqmultimap::key_iterator.html) |
| `QMultiMap` | keys | [QMultiMap](https://doc.qt.io/qt-6/qmultimap.html) |
| `QMultiMapIteratorJava` | -style const iterator for | [QMultiMapIteratorJava](https://doc.qt.io/qt-6/qmultimapiteratorjava.html) |
| `QMutableHashIteratorJava` | -style non-const iterator for | [QMutableHashIteratorJava](https://doc.qt.io/qt-6/qmutablehashiteratorjava.html) |
| `QMutableListIteratorJava` | -style non-const iterator for | [QMutableListIteratorJava](https://doc.qt.io/qt-6/qmutablelistiteratorjava.html) |
| `QMutableMapIteratorJava` | -style non-const iterator for | [QMutableMapIteratorJava](https://doc.qt.io/qt-6/qmutablemapiteratorjava.html) |
| `QMutableMultiMapIteratorJava` | -style non-const iterator for | [QMutableMultiMapIteratorJava](https://doc.qt.io/qt-6/qmutablemultimapiteratorjava.html) |
| `QMutableSetIteratorJava` | -style non-const iterator for | [QMutableSetIteratorJava](https://doc.qt.io/qt-6/qmutablesetiteratorjava.html) |
| `QMutexAccess` | serialization between threads | [QMutexAccess](https://doc.qt.io/qt-6/qmutexaccess.html) |
| `QMutexLockerConvenience` | class that simplifies locking and unlocking mutexes | [QMutexLockerConvenience](https://doc.qt.io/qt-6/qmutexlockerconvenience.html) |
| `QNativeInterface::QAndroidApplicationNative` | interface to a core application on Android | [QNativeInterface::QAndroidApplicationNative](https://doc.qt.io/qt-6/qnativeinterface::qandroidapplicationnative.html) |
| `QNativeIpcKeyHolds` | a native key used by | [QNativeIpcKeyHolds](https://doc.qt.io/qt-6/qnativeipckeyholds.html) |
| `QSystemSemaphore` | and | [QSystemSemaphore](https://doc.qt.io/qt-6/qsystemsemaphore.html) |
| `QNtfsPermissionCheckGuardRAII` | class to manage NTFS permission checking | [QNtfsPermissionCheckGuardRAII](https://doc.qt.io/qt-6/qntfspermissioncheckguardraii.html) |
| `QObjectThe` | base class of all | [QObjectThe](https://doc.qt.io/qt-6/qobjectthe.html) | 
| `QObjectBindablePropertyTemplate` | class that enables automatic property bindings for property data stored in | [QObjectBindablePropertyTemplate](https://doc.qt.io/qt-6/qobjectbindablepropertytemplate.html) |
| `QObject` | derived classes | [QObject](https://doc.qt.io/qt-6/qobject.html) |
| `QObjectCleanupHandlerWatches` | the lifetime of multiple | [QObjectCleanupHandlerWatches](https://doc.qt.io/qt-6/qobjectcleanuphandlerwatches.html) |
| `QObjectComputedPropertyTemplate` | class to help port old properties to the bindable property system | [QObjectComputedPropertyTemplate](https://doc.qt.io/qt-6/qobjectcomputedpropertytemplate.html) |
| `QOperatingSystemVersionInformation` | about the operating system version | [QOperatingSystemVersionInformation](https://doc.qt.io/qt-6/qoperatingsystemversioninformation.html) |    
| `QParallelAnimationGroupParallel` | group of animations | [QParallelAnimationGroupParallel](https://doc.qt.io/qt-6/qparallelanimationgroupparallel.html) |
| `QPartialOrderingRepresents` | the result of a comparison that allows for unordered results | [QPartialOrderingRepresents](https://doc.qt.io/qt-6/qpartialorderingrepresents.html) |  
| `QPauseAnimationPause` | for | [QPauseAnimationPause](https://doc.qt.io/qt-6/qpauseanimationpause.html) |
| `QPermissionAn` | opaque wrapper of a typed permission | [QPermissionAn](https://doc.qt.io/qt-6/qpermissionan.html) |
| `QPersistentModelIndexUsed` | to locate data in a data model | [QPersistentModelIndexUsed](https://doc.qt.io/qt-6/qpersistentmodelindexused.html) |
| `QPluginLoaderLoads` | a plugin at run-time | [QPluginLoaderLoads](https://doc.qt.io/qt-6/qpluginloaderloads.html) |
| `QPointDefines` | a point in the plane using integer precision | [QPointDefines](https://doc.qt.io/qt-6/qpointdefines.html) |
| `QPointFDefines` | a point in the plane using floating point precision | [QPointFDefines](https://doc.qt.io/qt-6/qpointfdefines.html) |
| `QPointerTemplate` | class that provides guarded pointers to | [QPointerTemplate](https://doc.qt.io/qt-6/qpointertemplate.html) |
| `QProcessUsed` | to start external programs and to communicate with them | [QProcessUsed](https://doc.qt.io/qt-6/qprocessused.html) |
| `QProcessEnvironmentHolds` | the environment variables that can be passed to a program | [QProcessEnvironmentHolds](https://doc.qt.io/qt-6/qprocessenvironmentholds.html) |
| `QPromiseWay` | to store computation results to be accessed by | [QPromiseWay](https://doc.qt.io/qt-6/qpromiseway.html) |
| `QPropertyTemplate` | class that enables automatic property bindings | [QPropertyTemplate](https://doc.qt.io/qt-6/qpropertytemplate.html) |
| `QPropertyChangeHandlerControls` | the lifecycle of change callback installed on a | [QPropertyChangeHandlerControls](https://doc.qt.io/qt-6/qpropertychangehandlercontrols.html) |   
| `QPropertyDataHelper` | class for properties with automatic property bindings | [QPropertyDataHelper](https://doc.qt.io/qt-6/qpropertydatahelper.html) |
| `QPropertyNotifierControls` | the lifecycle of change callback installed on a | [QPropertyNotifierControls](https://doc.qt.io/qt-6/qpropertynotifiercontrols.html) |
| `QQueueGeneric` | container that provides a queue | [QQueueGeneric](https://doc.qt.io/qt-6/qqueuegeneric.html) |
| `QRandomGeneratorAllows` | one to obtain random values from a high-quality Random Number Generator | [QRandomGeneratorAllows](https://doc.qt.io/qt-6/qrandomgeneratorallows.html) |   
| `QRandomGenerator64Allows` | one to obtain 64-bit random values from a high-quality, seed-less Random Number Generator | [QRandomGenerator64Allows](https://doc.qt.io/qt-6/qrandomgenerator64allows.html) |
| `QAbstractItemModel` | for any C++ range | [QAbstractItemModel](https://doc.qt.io/qt-6/qabstractitemmodel.html) |
| `QRangeModel::RowOptionsTemplate` | provides a customization point to control how | [QRangeModel::RowOptionsTemplate](https://doc.qt.io/qt-6/qrangemodel::rowoptionstemplate.html) |  
| `QRangeModel` | represents types used as rows | [QRangeModel](https://doc.qt.io/qt-6/qrangemodel.html) |
| `QReadLockerConvenience` | class that simplifies locking and unlocking read-write locks for read access | [QReadLockerConvenience](https://doc.qt.io/qt-6/qreadlockerconvenience.html) |
| `QReadWriteLockRead` | -write locking | [QReadWriteLockRead](https://doc.qt.io/qt-6/qreadwritelockread.html) |
| `QRectDefines` | a rectangle in the plane using integer precision | [QRectDefines](https://doc.qt.io/qt-6/qrectdefines.html) |
| `QRectFDefines` | a finite rectangle in the plane using floating point precision | [QRectFDefines](https://doc.qt.io/qt-6/qrectfdefines.html) |
| `QRecursiveMutexAccess` | serialization between threads | [QRecursiveMutexAccess](https://doc.qt.io/qt-6/qrecursivemutexaccess.html) |
| `QRegularExpressionPattern` | matching using regular expressions | [QRegularExpressionPattern](https://doc.qt.io/qt-6/qregularexpressionpattern.html) |
| `QRegularExpressionMatchThe` | results of a matching a | [QRegularExpressionMatchThe](https://doc.qt.io/qt-6/qregularexpressionmatchthe.html) |
| `QRegularExpression` | against a string | [QRegularExpression](https://doc.qt.io/qt-6/qregularexpression.html) |
| `QRegularExpressionMatchIteratorIterator` | on the results of a global match of a | [QRegularExpressionMatchIteratorIterator](https://doc.qt.io/qt-6/qregularexpressionmatchiteratoriterator.html) |
| `QResourceInterface` | for reading directly from resources | [QResourceInterface](https://doc.qt.io/qt-6/qresourceinterface.html) |
| `QRunnableThe` | base class for all runnable objects | [QRunnableThe](https://doc.qt.io/qt-6/qrunnablethe.html) |
| `QSaveFileInterface` | for safely writing to files | [QSaveFileInterface](https://doc.qt.io/qt-6/qsavefileinterface.html) |
| `QScopeGuardProvides` | a scope guard for calling a function at the end of a scope | [QScopeGuardProvides](https://doc.qt.io/qt-6/qscopeguardprovides.html) |
| `QScopedArrayPointerStores` | a pointer to a dynamically allocated array of objects, and deletes it upon destruction | [QScopedArrayPointerStores](https://doc.qt.io/qt-6/qscopedarraypointerstores.html) |
| `QScopedPointerStores` | a pointer to a dynamically allocated object, and deletes it upon destruction | [QScopedPointerStores](https://doc.qt.io/qt-6/qscopedpointerstores.html) |    
| `QScopedPropertyUpdateGroupRAII` | class around | [QScopedPropertyUpdateGroupRAII](https://doc.qt.io/qt-6/qscopedpropertyupdategroupraii.html) |
| `Qt::beginPropertyUpdateGroup` | ()/ | [Qt::beginPropertyUpdateGroup](https://doc.qt.io/qt-6/qt::beginpropertyupdategroup.html) |
| `Qt::endPropertyUpdateGroup` | () | [Qt::endPropertyUpdateGroup](https://doc.qt.io/qt-6/qt::endpropertyupdategroup.html) |
| `QScopedValueRollbackResets` | a variable to its previous value on destruction | [QScopedValueRollbackResets](https://doc.qt.io/qt-6/qscopedvaluerollbackresets.html) |
| `QSemaphoreGeneral` | counting semaphore | [QSemaphoreGeneral](https://doc.qt.io/qt-6/qsemaphoregeneral.html) |
| `QSemaphoreReleaserException` | -safe deferral of a | [QSemaphoreReleaserException](https://doc.qt.io/qt-6/qsemaphorereleaserexception.html) |
| `QSemaphore::release` | () call | [QSemaphore::release](https://doc.qt.io/qt-6/qsemaphore::release.html) |
| `QSequentialAnimationGroupSequential` | group of animations | [QSequentialAnimationGroupSequential](https://doc.qt.io/qt-6/qsequentialanimationgroupsequential.html) |
| `QSequentialIterableIterable` | interface for a container in a | [QSequentialIterableIterable](https://doc.qt.io/qt-6/qsequentialiterableiterable.html) |
| `QSetTemplate` | class that provides a hash-table-based set | [QSetTemplate](https://doc.qt.io/qt-6/qsettemplate.html) |
| `QSet::const_iteratorQSet::const_iterator` | class provides an STL-style const iterator for | [QSet::const_iteratorQSet::const_iterator](https://doc.qt.io/qt-6/qset::const_iteratorqset::const_iterator.html) |
| `QSet::iteratorQSet::iterator` | class provides an STL-style non-const iterator for | [QSet::iteratorQSet::iterator](https://doc.qt.io/qt-6/qset::iteratorqset::iterator.html) |      
| `QSetIteratorJava` | -style const iterator for | [QSetIteratorJava](https://doc.qt.io/qt-6/qsetiteratorjava.html) |
| `QSettingsPersistent` | platform-independent application settings | [QSettingsPersistent](https://doc.qt.io/qt-6/qsettingspersistent.html) |
| `QSharedDataBase` | class for shared data objects | [QSharedDataBase](https://doc.qt.io/qt-6/qshareddatabase.html) |
| `QSharedDataPointerRepresents` | a pointer to an implicitly shared object | [QSharedDataPointerRepresents](https://doc.qt.io/qt-6/qshareddatapointerrepresents.html) |
| `QSharedMemoryAccess` | to a shared memory segment | [QSharedMemoryAccess](https://doc.qt.io/qt-6/qsharedmemoryaccess.html) |
| `QSharedPointerHolds` | a strong reference to a shared pointer | [QSharedPointerHolds](https://doc.qt.io/qt-6/qsharedpointerholds.html) |
| `QSignalBlockerException` | -safe wrapper around | [QSignalBlockerException](https://doc.qt.io/qt-6/qsignalblockerexception.html) |
| `QObject::blockSignals` | () | [QObject::blockSignals](https://doc.qt.io/qt-6/qobject::blocksignals.html) |
| `QSignalMapperBundles` | signals from identifiable senders | [QSignalMapperBundles](https://doc.qt.io/qt-6/qsignalmapperbundles.html) |
| `QSizeDefines` | the size of a two-dimensional object using integer point precision | [QSizeDefines](https://doc.qt.io/qt-6/qsizedefines.html) |
| `QSizeFDefines` | the size of a two-dimensional object using floating point precision | [QSizeFDefines](https://doc.qt.io/qt-6/qsizefdefines.html) |
| `QSocketNotifierSupport` | for monitoring activity on a file descriptor | [QSocketNotifierSupport](https://doc.qt.io/qt-6/qsocketnotifiersupport.html) |
| `QSortFilterProxyModelSupport` | for sorting and filtering data passed between another model and a view | [QSortFilterProxyModelSupport](https://doc.qt.io/qt-6/qsortfilterproxymodelsupport.html) |
| `QSpanA` | non-owning container over contiguous data | [QSpanA](https://doc.qt.io/qt-6/qspana.html) |
| `QStackTemplate` | class that provides a stack | [QStackTemplate](https://doc.qt.io/qt-6/qstacktemplate.html) |
| `QStandardPathsMethods` | for accessing standard paths | [QStandardPathsMethods](https://doc.qt.io/qt-6/qstandardpathsmethods.html) |
| `QStaticByteArrayMatcherCompile` | -time version of | [QStaticByteArrayMatcherCompile](https://doc.qt.io/qt-6/qstaticbytearraymatchercompile.html) |
| `QStaticLatin1StringMatcherCompile` | -time version of | [QStaticLatin1StringMatcherCompile](https://doc.qt.io/qt-6/qstaticlatin1stringmatchercompile.html) |
| `QStaticPluginStruct` | containing a reference to a static plugin instance together with its meta data | [QStaticPluginStruct](https://doc.qt.io/qt-6/qstaticpluginstruct.html) |     
| `QStorageInfoProvides` | information about currently mounted storage and drives | [QStorageInfoProvides](https://doc.qt.io/qt-6/qstorageinfoprovides.html) |
| `QStringUnicode` | character string | [QStringUnicode](https://doc.qt.io/qt-6/qstringunicode.html) |
| `QStringConverterBase` | class for encoding and decoding text | [QStringConverterBase](https://doc.qt.io/qt-6/qstringconverterbase.html) |
| `QStringDecoderState` | -based decoder for text | [QStringDecoderState](https://doc.qt.io/qt-6/qstringdecoderstate.html) |
| `QStringEncoderState` | -based encoder for text | [QStringEncoderState](https://doc.qt.io/qt-6/qstringencoderstate.html) |
| `QStringListList` | of strings | [QStringListList](https://doc.qt.io/qt-6/qstringlistlist.html) |
| `QStringListModelModel` | that supplies strings to views | [QStringListModelModel](https://doc.qt.io/qt-6/qstringlistmodelmodel.html) |
| `QStringMatcherHolds` | a sequence of characters that can be quickly matched in a Unicode string | [QStringMatcherHolds](https://doc.qt.io/qt-6/qstringmatcherholds.html) |
| `QStringTokenizerSplits` | strings into tokens along given separators | [QStringTokenizerSplits](https://doc.qt.io/qt-6/qstringtokenizersplits.html) |
| `QStringViewUnified` | view on UTF-16 strings with a read-only subset of the | [QStringViewUnified](https://doc.qt.io/qt-6/qstringviewunified.html) |
| `QSysInfoInformation` | about the system | [QSysInfoInformation](https://doc.qt.io/qt-6/qsysinfoinformation.html) |
| `QSystemSemaphoreGeneral` | counting system semaphore | [QSystemSemaphoreGeneral](https://doc.qt.io/qt-6/qsystemsemaphoregeneral.html) |
| `QTaggedIteratorTemplate` | class that wraps an iterator and exposes standard iterator traits | [QTaggedIteratorTemplate](https://doc.qt.io/qt-6/qtaggediteratortemplate.html) |      
| `QTemporaryDirCreates` | a unique directory for temporary use | [QTemporaryDirCreates](https://doc.qt.io/qt-6/qtemporarydircreates.html) |
| `QTemporaryFileI` | /O device that operates on temporary files | [QTemporaryFileI](https://doc.qt.io/qt-6/qtemporaryfilei.html) |
| `QTextBoundaryFinderWay` | of finding Unicode text boundaries in a string | [QTextBoundaryFinderWay](https://doc.qt.io/qt-6/qtextboundaryfinderway.html) |
| `QTextStreamConvenient` | interface for reading and writing text | [QTextStreamConvenient](https://doc.qt.io/qt-6/qtextstreamconvenient.html) |
| `QThreadPlatform` | -independent way to manage threads | [QThreadPlatform](https://doc.qt.io/qt-6/qthreadplatform.html) |
| `QThreadPoolManages` | a collection of | [QThreadPoolManages](https://doc.qt.io/qt-6/qthreadpoolmanages.html) |
| `QThreadStoragePer` | -thread data storage | [QThreadStoragePer](https://doc.qt.io/qt-6/qthreadstorageper.html) |
| `QTimeClock` | time functions | [QTimeClock](https://doc.qt.io/qt-6/qtimeclock.html) |    
| `QTimeLineTimeline` | for controlling animations | [QTimeLineTimeline](https://doc.qt.io/qt-6/qtimelinetimeline.html) |
| `QTimeZoneIdentifies` | how a time representation relates to UTC | [QTimeZoneIdentifies](https://doc.qt.io/qt-6/qtimezoneidentifies.html) |
| `QTimerRepetitive` | and single-shot timers | [QTimerRepetitive](https://doc.qt.io/qt-6/qtimerrepetitive.html) |
| `QTimerEventContains` | parameters that describe a timer event | [QTimerEventContains](https://doc.qt.io/qt-6/qtimereventcontains.html) |
| `QTranslatorInternationalization` | support for text output | [QTranslatorInternationalization](https://doc.qt.io/qt-6/qtranslatorinternationalization.html) |
| `QTransposeProxyModelThis` | proxy transposes the source model | [QTransposeProxyModelThis](https://doc.qt.io/qt-6/qtransposeproxymodelthis.html) |
| `QTypeRevisionContains` | a lightweight representation of a version number with two 8-bit segments, major and minor, either of which can be unknown | [QTypeRevisionContains](https://doc.qt.io/qt-6/qtyperevisioncontains.html) |
| `QUnhandledExceptionRepresents` | an unhandled exception in a | [QUnhandledExceptionRepresents](https://doc.qt.io/qt-6/qunhandledexceptionrepresents.html) |
| `QUntypedBindableUniform` | interface over bindable properties like | [QUntypedBindableUniform](https://doc.qt.io/qt-6/quntypedbindableuniform.html) |
| `QObjectBindableProperty` | of any type T | [QObjectBindableProperty](https://doc.qt.io/qt-6/qobjectbindableproperty.html) |
| `QUrlConvenient` | interface for working with URLs | [QUrlConvenient](https://doc.qt.io/qt-6/qurlconvenient.html) |
| `QUrlQueryWay` | to manipulate a key-value pairs in a URL's query | [QUrlQueryWay](https://doc.qt.io/qt-6/qurlqueryway.html) |
| `QUtf8StringViewUnified` | view on UTF-8 strings with a read-only subset of the | [QUtf8StringViewUnified](https://doc.qt.io/qt-6/qutf8stringviewunified.html) |
| `QUuidStores` | a Universally Unique Identifier (UUID) | [QUuidStores](https://doc.qt.io/qt-6/quuidstores.html) |
| `QVarLengthArrayLow` | -level variable-length array | [QVarLengthArrayLow](https://doc.qt.io/qt-6/qvarlengtharraylow.html) |
| `QVariantActs` | like a union for the most common | [QVariantActs](https://doc.qt.io/qt-6/qvariantacts.html) |
| `QVariant::ConstPointerQVariant::ConstPointer` | is a template class that emulates a const pointer to | [QVariant::ConstPointerQVariant::ConstPointer](https://doc.qt.io/qt-6/qvariant::constpointerqvariant::constpointer.html) |
| `QVariant::ConstReferenceQVariant::ConstReference` | acts as a const reference to a | [QVariant::ConstReferenceQVariant::ConstReference](https://doc.qt.io/qt-6/qvariant::constreferenceqvariant::constreference.html) |
| `QVariant::PointerQVariant::Pointer` | is a template class that emulates a non-const pointer to | [QVariant::PointerQVariant::Pointer](https://doc.qt.io/qt-6/qvariant::pointerqvariant::pointer.html) |
| `QVariant::ReferenceQVariant::Reference` | acts as a non-const reference to a | [QVariant::ReferenceQVariant::Reference](https://doc.qt.io/qt-6/qvariant::referenceqvariant::reference.html) |
| `QVariantAnimationBase` | class for animations | [QVariantAnimationBase](https://doc.qt.io/qt-6/qvariantanimationbase.html) |
| `QVariantConstPointerEmulated` | const pointer to | [QVariantConstPointerEmulated](https://doc.qt.io/qt-6/qvariantconstpointeremulated.html) |
| `QVariantPointerTemplate` | class that emulates a pointer to | [QVariantPointerTemplate](https://doc.qt.io/qt-6/qvariantpointertemplate.html) |
| `QVariantRefActs` | as a non-const reference to a | [QVariantRefActs](https://doc.qt.io/qt-6/qvariantrefacts.html) |
| `QVectorAlias` | for | [QVectorAlias](https://doc.qt.io/qt-6/qvectoralias.html) |
| `QVersionNumberContains` | a version number with an arbitrary number of segments | [QVersionNumberContains](https://doc.qt.io/qt-6/qversionnumbercontains.html) |
| `QWaitConditionCondition` | variable for synchronizing threads | [QWaitConditionCondition](https://doc.qt.io/qt-6/qwaitconditioncondition.html) |
| `QWeakPointerHolds` | a weak reference to a shared pointer | [QWeakPointerHolds](https://doc.qt.io/qt-6/qweakpointerholds.html) |
| `QWinEventNotifierSupport` | for the Windows Wait functions | [QWinEventNotifierSupport](https://doc.qt.io/qt-6/qwineventnotifiersupport.html) |
| `QWriteLockerConvenience` | class that simplifies locking and unlocking read-write locks for write access | [QWriteLockerConvenience](https://doc.qt.io/qt-6/qwritelockerconvenience.html) |
| `QXmlStreamAttributeRepresents` | a single XML attribute | [QXmlStreamAttributeRepresents](https://doc.qt.io/qt-6/qxmlstreamattributerepresents.html) |
| `QXmlStreamAttributesRepresents` | a vector of | [QXmlStreamAttributesRepresents](https://doc.qt.io/qt-6/qxmlstreamattributesrepresents.html) |
| `QXmlStreamEntityDeclarationRepresents` | a DTD entity declaration | [QXmlStreamEntityDeclarationRepresents](https://doc.qt.io/qt-6/qxmlstreamentitydeclarationrepresents.html) |     
| `QXmlStreamEntityResolverEntity` | resolver for a | [QXmlStreamEntityResolverEntity](https://doc.qt.io/qt-6/qxmlstreamentityresolverentity.html) |
| `QXmlStreamNamespaceDeclarationRepresents` | a namespace declaration | [QXmlStreamNamespaceDeclarationRepresents](https://doc.qt.io/qt-6/qxmlstreamnamespacedeclarationrepresents.html) |
| `QXmlStreamNotationDeclarationRepresents` | a DTD notation declaration | [QXmlStreamNotationDeclarationRepresents](https://doc.qt.io/qt-6/qxmlstreamnotationdeclarationrepresents.html) |
| `QXmlStreamReaderFast` | parser for reading well-formed XML 1.0 documents via a simple streaming API | [QXmlStreamReaderFast](https://doc.qt.io/qt-6/qxmlstreamreaderfast.html) |     
| `QXmlStreamWriterXML` | 1.0 writer with a simple streaming API | [QXmlStreamWriterXML](https://doc.qt.io/qt-6/qxmlstreamwriterxml.html) |
| `Qt::partial_orderingQt::partial_ordering` | represents the result of a comparison that allows for unordered results | [Qt::partial_orderingQt::partial_ordering](https://doc.qt.io/qt-6/qt::partial_orderingqt::partial_ordering.html) |
| `Qt::strong_orderingQt::strong_ordering` | represents a comparison where equivalent values are indistinguishable | [Qt::strong_orderingQt::strong_ordering](https://doc.qt.io/qt-6/qt::strong_orderingqt::strong_ordering.html) |
| `Qt::totally_ordered_wrapperQt::totally_ordered_wrapper` | is a wrapper type that provides strict total order for the wrapped types | [Qt::totally_ordered_wrapperQt::totally_ordered_wrapper](https://doc.qt.io/qt-6/qt::totally_ordered_wrapperqt::totally_ordered_wrapper.html) |
| `Qt::weak_orderingQt::weak_ordering` | represents a comparison where equivalent values are still distinguishable | [Qt::weak_orderingQt::weak_ordering](https://doc.qt.io/qt-6/qt::weak_orderingqt::weak_ordering.html) |
| `QtFuture::WhenAnyResultQtFuture::WhenAnyResult` | is used to represent the result of | [QtFuture::WhenAnyResultQtFuture::WhenAnyResult](https://doc.qt.io/qt-6/qtfuture::whenanyresultqtfuture::whenanyresult.html) |
| `QtFuture::whenAny` | () | [QtFuture::whenAny](https://doc.qt.io/qt-6/qtfuture::whenany.html) |
